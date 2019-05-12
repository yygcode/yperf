#
# yperf - yperf test suites
#
# Copyright (C) 2018 yonggang.yyg<yygcode@gmail>
#

if (CMAKE_COMPILER_IS_GNUCXX)
  list(APPEND CMAKE_CXX_FLAGS -std=c++11)
  set(CMAKE_CXX_FLAGS_RELEASE -O2)
  set(CMAKE_CXX_FLAGS_DEBUG -O0)
  message(STATUS "c++ use -std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)

# depends third-part library
set(TEST_LIBS pthread m rt gtest gmock)
generic_find_multi_library(${TEST_LIBS})

list(APPEND TEST_LIBS yperf)

add_subdirectory(lock)
