#
# yperf - yperf test suites
#
# Copyright (C) 2018 yonggang.yyg<yygcode@gmail>
#

project(yperf)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/include)

if (CMAKE_COMPILER_IS_GNUCXX)
  # add_cxx_compile_options(-std=c++11)
  set(CMAKE_CXX_FLAGS -std=c++11)
  set(CMAKE_CXX_FLAGS_RELEASE -O2)
  set(CMAKE_CXX_FLAGS_DEBUG -O0)
  message(STATUS "c++ -std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(CHECK_DEP_LIBS subunit m rt)
set(GTEST_LIBS gtest gmock)
set(YPERF_LIB yperf)
set(TEST_LIBS ${CHECK_DEP_LIBS} ${GTEST_LIBS} ${THREAD_LIBS} ${YPERF_LIB})

add_subdirectory(lock)

list(APPEND test-targets test-lock)

add_custom_target(run-tests
  COMMAND ${test-targets})

add_dependencies(run-tests ${test-targets})
