#
# yperf - A parallel performance library
#
# Copyright (C) 2018 yonggang.yyg<yygcode@gmail>
#

project(yperf C)

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)
# set(CMAKE_BUILD_TYPE Debug)
set(THREAD_LIBS pthread)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
  add_compile_options(-DDEBUG)
endif(CMAKE_BUILD_TYPE MATCHES "Debug")

if (CMAKE_BUILD_TYPE MATCHES "Release")
  add_compile_options(-DNDEBUG)
endif(CMAKE_BUILD_TYPE MATCHES "Release")

message(STATUS "Build mode " ${CMAKE_BUILD_TYPE})

add_compile_options(-Wall -Wextra -Werror -Wconversion -Wsign-conversion)
add_compile_options(-D_GNU_SOURCE)

include(CheckIncludeFile)

add_subdirectory(src)
add_subdirectory(test)
