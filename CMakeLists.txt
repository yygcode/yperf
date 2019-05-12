#
# yperf - A basic performance library
#
# Copyright (C) 2018 yonggang.yyg<yygcode@gmail>
#

project(yperf C CXX)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include (CheckLibraryExists)
include(generic)

# option list
option(BUILD_SHARED_LIBS "Build the shared library" ON)

# all targets need this include
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/include)

# in generic
generic_add_compile_options_debug_or_release()
generic_add_compile_options_warning()

include(CheckIncludeFile)

add_subdirectory(src)

# before add_test
enable_testing()
add_subdirectory(test)
