#
# yperf - yperf test suites for lock
#
# Copyright (C) 2018 yonggang.yyg<yygcode@gmail>
#

# Just an example of libcheck.
# Use googletest for pretty output and efficiency
set(TEST_CHECK_LOCK_SRCS
  test-check.c
  test-check-atomic.c
  test-check-atomic64.c
  threadpool.c)

# add_executable(test-check-lock ${TEST_CHECK_LOCK_SRCS})
# target_link_libraries(test-check-lock check ${CHECK_DEP_LIBS})

# list(APPEND CMAKE_CXX_SOURCE_FILE_EXTENSIONS cc)

set(TEST_LOCK_SRCS
  test-gtest.cc
  test-atomic.cc
  test-atomic64.cc
  test-cmpxchg.cc
  test-spinlock.cc
  threadpool.c)

add_executable(test-lock ${TEST_LOCK_SRCS})
target_link_libraries(test-lock ${GTEST_LIBS} ${THREAD_LIBS})
# set_source_files_properties(threadpool.c PROPERTIES LANGUAGE CXX)
# SET_SOURCE_FILES_PROPERTIES( )

#list(APPEND test-targets test-lock PARENT_SCOPE)

add_custom_target(run-test-lock
  COMMAND test-lock)

add_dependencies(run-test-lock test-lock)
