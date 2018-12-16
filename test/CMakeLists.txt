#
# yperf - yperf test suites
#
# Copyright (C) 2018 yonggang.yyg<yygcode@gmail>
#

project(yperf)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/include)

set(CHECK_DEP_LIBS subunit m rt)
set(GTEST_LIBS gtest gmock)

add_subdirectory(lock)

list(APPEND test-targets test-lock)

add_custom_target(run-tests
  COMMAND ${test-targets})

add_dependencies(run-tests ${test-targets})
