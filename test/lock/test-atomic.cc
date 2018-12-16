#include <gtest/gtest.h>
// Expect all fails if no lock prefix
// #define YPERF_TEST
// #define Y_LOCK_PREFIX ""
#include <yperf/atomic.h>
#include <yperf/barrier.h>

#include "threadpool.h"

namespace
{

class TestAtomic : public testing::Test
{
public:
    TestAtomic() {}
    virtual ~TestAtomic() {}

    void SetUp() override
    {
        y_atomic_set(&mAtomic, 0);
        EXPECT_EQ(y_atomic_read(&mAtomic), 0);
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
        TestAtomic *p = static_cast<TestAtomic*>(arg);

        while (!p->mReady);
        for (size_t i = 0; i < OP_COUNT; ++i) {
            y_atomic_inc(&p->mAtomic);
        }
    }
    static void test_op_dec(void *arg)
    {
        TestAtomic *p = static_cast<TestAtomic*>(arg);

        while (!p->mReady);
        for (size_t i = 0; i < OP_COUNT; ++i) {
            y_atomic_dec(&p->mAtomic);
        }
    }
    static void test_op_add(void *arg)
    {
        TestAtomic *p = static_cast<TestAtomic*>(arg);

        while (!p->mReady);
        for (size_t i = 0; i < OP_COUNT; ++i) {
            y_atomic_add(&p->mAtomic, OP_STEPS);
        }
    }
    static void test_op_sub(void *arg)
    {
        TestAtomic *p = static_cast<TestAtomic*>(arg);

        while (!p->mReady);
        for (size_t i = 0; i < OP_COUNT; ++i) {
            y_atomic_sub(&p->mAtomic, OP_STEPS);
        }
    }
    static void test_op_dec_and_test(void *arg)
    {
        TestAtomic *p = static_cast<TestAtomic*>(arg);

        while (!p->mReady);
        for (size_t i = 0; i < OP_COUNT; ++i) {
            if (y_atomic_dec_and_test(&p->mAtomic)) {
                y_atomic_inc(&p->mOpAndTest);
            }
        }
    }
    static void test_op_sub_and_test(void *arg)
    {
        TestAtomic *p = static_cast<TestAtomic*>(arg);

        while (!p->mReady);
        for (size_t i = 0; i < OP_COUNT; ++i) {
            if (y_atomic_sub_and_test(&p->mAtomic, OP_STEPS)) {
                y_atomic_inc(&p->mOpAndTest);
            }
        }
    }

protected:
    const static size_t THREAD_COUNT = 20;
    const static int OP_COUNT = 1000000;
    const static int OP_STEPS = 3;

protected:
    volatile bool mReady;
    y_atomic_t mAtomic;
    y_atomic_t mOpAndTest;
    test_threadpool_t *mPool;
};

TEST_F(TestAtomic, inc)
{
    ExecTask(test_op_inc);
    EXPECT_EQ(y_atomic_read(&mAtomic), THREAD_COUNT * OP_COUNT);
}

TEST_F(TestAtomic, dec)
{
    y_atomic_set(&mAtomic, THREAD_COUNT * OP_COUNT + 10);
    EXPECT_EQ(y_atomic_read(&mAtomic), THREAD_COUNT * OP_COUNT + 10);
    ExecTask(test_op_dec);
    EXPECT_EQ(y_atomic_read(&mAtomic), 10);
}

TEST_F(TestAtomic, add)
{
    ExecTask(test_op_add);
    EXPECT_EQ(y_atomic_read(&mAtomic), THREAD_COUNT * OP_COUNT * OP_STEPS);
}

TEST_F(TestAtomic, sub)
{
    y_atomic_set(&mAtomic, THREAD_COUNT * OP_COUNT * OP_STEPS + 6);
    EXPECT_EQ(y_atomic_read(&mAtomic), THREAD_COUNT * OP_COUNT * OP_STEPS + 6);
    ExecTask(test_op_sub);
    EXPECT_EQ(y_atomic_read(&mAtomic), 6);
}

TEST_F(TestAtomic, dec_and_test)
{
    y_atomic_set(&mAtomic, THREAD_COUNT * OP_COUNT - 1000);
    y_atomic_set(&mOpAndTest, 0);
    ExecTask(test_op_dec_and_test);
    EXPECT_EQ(y_atomic_read(&mAtomic), -1000);
    EXPECT_EQ(y_atomic_read(&mOpAndTest), 1);
}

TEST_F(TestAtomic, sub_and_test)
{
    y_atomic_set(&mAtomic, (THREAD_COUNT * OP_COUNT - 1000) * OP_STEPS);
    y_atomic_set(&mOpAndTest, 0);
    ExecTask(test_op_sub_and_test);
    EXPECT_EQ(y_atomic_read(&mAtomic), -1000 * OP_STEPS);
    EXPECT_EQ(y_atomic_read(&mOpAndTest), 1);
}

} // namespace
