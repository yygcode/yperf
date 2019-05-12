# Common for all projects

function (generic_add_compile_options_debug_or_release)
    string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE)
    if (CMAKE_BUILD_TYPE MATCHES "debug")
        add_compile_options(-DDEBUG)
        message(STATUS "Compile in DEBUG mode (-DDEBUG)")
    elseif (CMAKE_BUILD_TYPE MATCHES "release" OR
            CMAKE_BUILD_TYPE STREQUAL "")
        add_compile_options(-DNDEBUG)
        message(STATUS "Compile in RELEASE mode (-DNDEBUG)")
    else()
        message(FATAL_ERROR "Unkown CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}. "
            "Valid value is 'debug' or 'release'.")
    endif()
endfunction()

function (generic_add_compile_options_warning)
    add_compile_options(-Wall -Wextra -Werror -Wconversion -Wsign-conversion)
endfunction()

function(generic_find_multi_library)
    foreach(arg ${ARGN})
        find_library(LIB_${arg} "${arg}")
        if (NOT LIB_${arg})
            message(FATAL_ERROR "Could not find library ${arg}")
        endif()
        message(STATUS "Library ${arg} use ${LIB_${arg}}")
    endforeach()
endfunction()
