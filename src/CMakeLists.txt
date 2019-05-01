#
# yperf - Basic performance library
#
# Copyright (C) 2018 yonggang.yyg<yygcode@gmail>
#

project(yperf
  LANGUAGES C
  VERSION 0.1.1
  # DESCRIPTION "performance library"
  )

# cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (CMAKE_COMPILER_IS_GNUCC)
  add_compile_options(-std=gnu99)
  message(STATUS "cc -std=gnu99")
endif(CMAKE_COMPILER_IS_GNUCC)

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(locking)

# target_link_library();
set(SRCS
  ${LOCKING_SRCS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(yperf SHARED ${SRCS})
# add_library(yperf-a STATIC ${SRCS})

set_target_properties(yperf PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(yperf PROPERTIES SOVERSION 1)
set_target_properties(yperf PROPERTIES PUBLIC_HEADER ./include)


include(CheckCSourceRuns)
