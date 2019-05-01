#include <limits.h>

#include <gtest/gtest.h>
// Expect all fails if no lock prefix
// #define YPERF_TEST
// #define Y_LOCK_PREFIX ""
#include <yperf/base.h>

#include "threadpool.h"

namespace
{

class TestCmpxchg : public testing::Test
{
public:
    TestCmpxchg() {}
    virtual ~TestCmpxchg() {}

    void SetUp() override
    {
        mVal = 0;
        mVal64 = 0;
    }

    void TearDown() override
    {
    }

protected:
    volatile uint32_t mVal;
    volatile uint64_t mVal64;
};

TEST_F(TestCmpxchg, val)
{
    bool ret;

    EXPECT_EQ(mVal, 0);

    ret = y_cmpxchg(&mVal, 0, 1);
    EXPECT_EQ(mVal, 1);
    EXPECT_EQ(ret, true);

    ret = y_cmpxchg(&mVal, 0, 2);
    EXPECT_EQ(mVal, 1);
    EXPECT_EQ(ret, false);

    ret = y_cmpxchg(&mVal, 1, 5);
    EXPECT_EQ(mVal, 5);
    EXPECT_EQ(ret, true);
}

TEST_F(TestCmpxchg, val64)
{
    bool ret;

    EXPECT_EQ(mVal64, 0);

    ret = y_cmpxchg64(&mVal64, 0, UINT64_MAX - 1000);
    EXPECT_EQ(mVal64, UINT64_MAX - 1000);
    EXPECT_EQ(ret, true);

    ret = y_cmpxchg64(&mVal64, 0, 2);
    EXPECT_EQ(mVal64, UINT64_MAX - 1000);
    EXPECT_EQ(ret, false);

    ret = y_cmpxchg64(&mVal64, UINT64_MAX - 1000, 5);
    EXPECT_EQ(mVal64, 5);
    EXPECT_EQ(ret, true);
}

} // namespace
