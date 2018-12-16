#pragma once

extern SRunner *test_sr;

#define test_add_suite(s)               \
    ({ typeof(s) __test_suite_ss = s;   \
        srunner_add_suite()             \
    })
