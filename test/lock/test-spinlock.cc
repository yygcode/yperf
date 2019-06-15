#include <limits.h>

#include <gtest/gtest.h>
// Expect all fails if no lock prefix
// #define YPERF_TEST
// #define Y_LOCK_PREFIX ""
#include <yperf/spinlock.h>

#include "threadpool.h"

namespace
{

class TestSpinlock : public testing::Test
{
public:
    TestSpinlock() {}
    virtual ~TestSpinlock() {}

    void SetUp() override
    {
        mVal = 0;
        y_spinlock_init(&mLock);
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
    static void test_op(void *arg)
    {
        TestSpinlock *p = static_cast<TestSpinlock*>(arg);

        while (!p->mReady);
        for (size_t i = 0; i < OP_COUNT; ++i) {
            y_spinlock_lock(&p->mLock);
            ++p->mVal;
            y_spinlock_unlock(&p->mLock);
        }
    }

protected:
    const static size_t THREAD_COUNT = 20;
    const static int OP_COUNT = 100000;

protected:
    bool mReady;
    uint32_t mVal;
    y_spinlock_t mLock;
    test_threadpool_t *mPool;
};

TEST_F(TestSpinlock, init)
{
    bool ret;

    y_spinlock_t lock_unlocked = Y_SPINLOCK_INIT();
    y_spinlock_t lock_locked = Y_SPINLOCK_INIT_LOCKED();
    EXPECT_EQ(lock_unlocked.lockval, Y_SPINLOCK_UNLOCKEDV);
    EXPECT_EQ(lock_locked.lockval, Y_SPINLOCK_LOCKEDV);

    ret = y_spinlock_trylock(&lock_unlocked);
    EXPECT_EQ(ret, true);
    ret = y_spinlock_trylock(&lock_locked);
    EXPECT_EQ(ret, false);

    y_spinlock_init(&lock_unlocked);
    y_spinlock_init_locked(&lock_locked);
    ret = y_spinlock_trylock(&lock_unlocked);
    EXPECT_EQ(ret, true);
    ret = y_spinlock_trylock(&lock_locked);
    EXPECT_EQ(ret, false);

    ret = y_spinlock_is_locked(&lock_locked);
    EXPECT_EQ(ret, true);
    ret = y_spinlock_is_locked(&lock_unlocked);
    EXPECT_EQ(ret, true);
}

TEST_F(TestSpinlock, lockunlock)
{
    bool ret;

    ExecTask(test_op);
    EXPECT_EQ(mVal, THREAD_COUNT * OP_COUNT);

    ExecTask(test_op);
    EXPECT_EQ(mVal, THREAD_COUNT * OP_COUNT * 2);

    ret = y_spinlock_is_locked(&mLock);
    EXPECT_EQ(ret, false);
}

TEST_F(TestSpinlock, trylock)
{
    bool ret;

    ret = y_spinlock_trylock(&mLock);
    EXPECT_EQ(ret, true);

    ret = y_spinlock_trylock(&mLock);
    EXPECT_EQ(ret, false);

    y_spinlock_unlock(&mLock);
    ret = y_spinlock_trylock(&mLock);
    EXPECT_EQ(ret, true);
    ret = y_spinlock_is_locked(&mLock);
    EXPECT_EQ(ret, true);

    y_spinlock_unlock(&mLock);
    ret = y_spinlock_is_locked(&mLock);
    EXPECT_EQ(ret, false);
}

} // namespace
