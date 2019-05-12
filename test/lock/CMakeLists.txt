#
# yperf - yperf test suites for lock
#
# Copyright (C) 2018 yonggang.yyg<yygcode@gmail>
#

set(TEST_LOCK_SRCS
  test-gtest.cc
  test-atomic.cc
  test-atomic64.cc
  test-cmpxchg.cc
  test-spinlock.cc
  threadpool.c)

add_executable(test-lock ${TEST_LOCK_SRCS})
target_link_libraries(test-lock ${TEST_LIBS})

add_test(NAME test-yperf-lock COMMAND test-lock)
