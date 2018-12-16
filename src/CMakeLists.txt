#
# yperf - A parallel performance library
#
# Copyright (C) 2018 yonggang.yyg<yygcode@gmail>
#
#

project(yperf C)
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_BUILD_TYPE Debug)

# add_library(yperf STATIC)
# add_library(yperf SHARED)

# target_link_library();

include(CheckCSourceRuns)
