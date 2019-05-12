#
# yperf - Basic performance library
#
# Copyright (C) 2018 yonggang.yyg<yygcode@gmail>
#

if (CMAKE_COMPILER_IS_GNUCC)
    add_compile_options(-std=gnu99)
    message(STATUS "cc -std=gnu99")
endif(CMAKE_COMPILER_IS_GNUCC)

add_subdirectory(locking)

# target_link_library();
set(SRCS ${LOCKING_SRCS})

add_library(yperf ${SRCS})
set_target_properties(yperf PROPERTIES VERSION 0.1 SOVERSION 1)
set_target_properties(yperf PROPERTIES PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(CheckCSourceRuns)
