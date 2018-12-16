#include <limits.h>

#include <gtest/gtest.h>
// Expect all fails if no lock prefix
// #define YPERF_TEST
// #define Y_LOCK_PREFIX ""
#include <yperf/atomic64.h>
#include <yperf/barrier.h>

#include "threadpool.h"

namespace
{

class TestAtomic64 : public testing::Test
{
public:
    TestAtomic64() {}
    virtual ~TestAtomic64() {}

    void SetUp() override
    {
        y_atomic64_set(&mAtomic, OP_OFFSET);
        EXPECT_EQ(y_atomic64_read(&mAtomic), OP_OFFSET + 0);
        mPool = test_threadpool_create(THREAD_COUNT);
        mReady = false;
    }

    void TearDown() override
    {
        test_threadpool_destroy(mPool);
    }

protected:
    void ExecTask(void (*func)(void*))
    {
        mReady = false;
        y_barrier();
        test_threadpool_add_task_for_each(mPool, func, this);
        y_barrier();
        mReady = true;

        test_threadpool_wait_all(mPool);
    }

protected:
    // Thread functions
    static void test_op_inc(void *arg)
    {
        TestAtomic64 *p = static_cast<TestAtomic64*>(arg);

        while (!p->mReady);
        for (size_t i = 0; i < OP_COUNT; ++i) {
            y_atomic64_inc(&p->mAtomic);
        }
    }
    static void test_op_dec(void *arg)
    {
        TestAtomic64 *p = static_cast<TestAtomic64*>(arg);

        while (!p->mReady);
        for (size_t i = 0; i < OP_COUNT; ++i) {
            y_atomic64_dec(&p->mAtomic);
        }
    }
    static void test_op_add(void *arg)
    {
        TestAtomic64 *p = static_cast<TestAtomic64*>(arg);

        while (!p->mReady);
        for (size_t i = 0; i < OP_COUNT; ++i) {
            y_atomic64_add(&p->mAtomic, OP_STEPS);
        }
    }
    static void test_op_sub(void *arg)
    {
        TestAtomic64 *p = static_cast<TestAtomic64*>(arg);

        while (!p->mReady);
        for (size_t i = 0; i < OP_COUNT; ++i) {
            y_atomic64_sub(&p->mAtomic, OP_STEPS);
        }
    }
    static void test_op_dec_and_test(void *arg)
    {
        TestAtomic64 *p = static_cast<TestAtomic64*>(arg);

        while (!p->mReady);
        for (size_t i = 0; i < OP_COUNT; ++i) {
            if (y_atomic64_dec_and_test(&p->mAtomic)) {
                y_atomic64_inc(&p->mOpAndTest);
            }
        }
    }
    static void test_op_sub_and_test(void *arg)
    {
        TestAtomic64 *p = static_cast<TestAtomic64*>(arg);

        while (!p->mReady);
        for (size_t i = 0; i < OP_COUNT; ++i) {
            if (y_atomic64_sub_and_test(&p->mAtomic, OP_STEPS)) {
                y_atomic64_inc(&p->mOpAndTest);
            }
        }
    }

protected:
    const static size_t THREAD_COUNT = 20;
    const static long OP_COUNT = 1000000;
    const static long OP_STEPS = 3;
    const static long OP_OFFSET = (long)UINT_MAX * OP_STEPS;

protected:
    volatile bool mReady;
    y_atomic64_t mAtomic;
    y_atomic64_t mOpAndTest;
    test_threadpool_t *mPool;
};

TEST_F(TestAtomic64, inc)
{
    ExecTask(test_op_inc);
    EXPECT_EQ(y_atomic64_read(&mAtomic), THREAD_COUNT * OP_COUNT + OP_OFFSET);
}

TEST_F(TestAtomic64, dec)
{
    y_atomic64_set(&mAtomic, THREAD_COUNT * OP_COUNT + OP_OFFSET);
    EXPECT_EQ(y_atomic64_read(&mAtomic), THREAD_COUNT * OP_COUNT + OP_OFFSET);
    ExecTask(test_op_dec);
    EXPECT_EQ(y_atomic64_read(&mAtomic), OP_OFFSET + 0);
}

TEST_F(TestAtomic64, add)
{
    ExecTask(test_op_add);
    EXPECT_EQ(y_atomic64_read(&mAtomic),
              THREAD_COUNT * OP_COUNT * OP_STEPS + OP_OFFSET);
}

TEST_F(TestAtomic64, sub)
{
    y_atomic64_set(&mAtomic, THREAD_COUNT * OP_COUNT * OP_STEPS + OP_OFFSET);
    EXPECT_EQ(y_atomic64_read(&mAtomic),
              THREAD_COUNT * OP_COUNT * OP_STEPS + OP_OFFSET);
    ExecTask(test_op_sub);
    EXPECT_EQ(y_atomic64_read(&mAtomic), OP_OFFSET + 0);
}

TEST_F(TestAtomic64, dec_and_test)
{
    y_atomic64_set(&mAtomic, THREAD_COUNT * OP_COUNT - 1000);
    y_atomic64_set(&mOpAndTest, OP_OFFSET);
    ExecTask(test_op_dec_and_test);
    EXPECT_EQ(y_atomic64_read(&mAtomic), -1000);
    EXPECT_EQ(y_atomic64_read(&mOpAndTest), OP_OFFSET + 1);
}

TEST_F(TestAtomic64, sub_and_test)
{
    y_atomic64_set(&mAtomic,
                   (THREAD_COUNT * OP_COUNT - 1000) * OP_STEPS);
    y_atomic64_set(&mOpAndTest, 0);
    ExecTask(test_op_sub_and_test);
    EXPECT_EQ(y_atomic64_read(&mAtomic), -1000 * OP_STEPS);
    EXPECT_EQ(y_atomic64_read(&mOpAndTest), 1);
}

} // namespace
