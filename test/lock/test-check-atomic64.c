#include <check.h>
#include <pthread.h>
#include <stdio.h>
#include <limits.h>

#include <yperf/atomic64.h>
#include <yperf/barrier.h>

#include "threadpool.h"

/* Create a basic thread pool to test atomic64 operation */
#define ATOMIC64_OP_COUNT       (1000*100L)
#define ATOMIC64_THREAD_COUNT   (20U)
#define ATOMIC64_ADD_STEP       (1000L)

static volatile bool thread_func_run_flag = false;
static void thread_func_atomic64_inc(void *arg)
{
    y_atomic64_t *x = (y_atomic64_t*)arg;

    while (!thread_func_run_flag); /* busy wait */

    for (size_t i = 0; i < ATOMIC64_OP_COUNT; ++i) {
        y_atomic64_inc(x);
    }
}

static void thread_func_atomic64_dec(void *arg)
{
    y_atomic64_t *x = (y_atomic64_t*)arg;

    while (!thread_func_run_flag); /* busy wait */

    for (size_t i = 0; i < ATOMIC64_OP_COUNT; ++i) {
        y_atomic64_dec(x);
    }
}

static void thread_func_atomic64_add(void *arg)
{
    y_atomic64_t *x = (y_atomic64_t*)arg;

    while (!thread_func_run_flag); /* busy wait */

    for (size_t i = 0; i < ATOMIC64_OP_COUNT; ++i) {
        y_atomic64_add(x, ATOMIC64_ADD_STEP);
    }
}

static void thread_func_atomic64_sub(void *arg)
{
    y_atomic64_t *x = (y_atomic64_t*)arg;

    while (!thread_func_run_flag); /* busy wait */

    for (size_t i = 0; i < ATOMIC64_OP_COUNT; ++i) {
        y_atomic64_sub(x, ATOMIC64_ADD_STEP);
    }
}

static void thread_func_atomic64_dec_and_test(void *arg)
{
    y_atomic64_t *x = (y_atomic64_t*)arg;
    y_atomic64_t *y = x + 1;

    while (!thread_func_run_flag); /* busy wait */

    for (size_t i = 0; i < ATOMIC64_OP_COUNT; ++i) {
        if (y_atomic64_dec_and_test(x)) {
            y_atomic64_inc(y);
        }
    }
}

static void thread_func_atomic64_sub_and_test(void *arg)
{
    y_atomic64_t *x = (y_atomic64_t*)arg;
    y_atomic64_t *y = x + 1;

    while (!thread_func_run_flag); /* busy wait */

    for (size_t i = 0; i < ATOMIC64_OP_COUNT; ++i) {
        if (y_atomic64_sub_and_test(x, ATOMIC64_ADD_STEP)) {
            y_atomic64_inc(y);
        }
    }
}

START_TEST(test_atomic64_inc)
{
    y_atomic64_t x = Y_ATOMIC64_INIT(UINT_MAX);
    test_threadpool_t *pool = test_threadpool_create(ATOMIC64_THREAD_COUNT);

    ck_assert(pool);

    thread_func_run_flag = false;
    y_barrier();

    test_threadpool_add_task_for_each(pool, thread_func_atomic64_inc, &x);
    test_threadpool_add_task_for_each(pool, thread_func_atomic64_inc, &x);

    y_barrier();
    thread_func_run_flag = true;

    test_threadpool_destroy(pool);

    ck_assert(y_atomic64_read(&x) ==
              ATOMIC64_THREAD_COUNT * ATOMIC64_OP_COUNT * 2 + UINT_MAX);
}
END_TEST

START_TEST(test_atomic64_dec)
{
    y_atomic64_t x = Y_ATOMIC64_INIT(ATOMIC64_THREAD_COUNT * ATOMIC64_OP_COUNT * 2);
    test_threadpool_t *pool = test_threadpool_create(ATOMIC64_THREAD_COUNT);

    ck_assert(pool);

    thread_func_run_flag = false;
    y_barrier();

    test_threadpool_add_task_for_each(pool, thread_func_atomic64_dec, &x);
    test_threadpool_add_task_for_each(pool, thread_func_atomic64_dec, &x);

    y_barrier();
    thread_func_run_flag = true;

    test_threadpool_destroy(pool);

    ck_assert(y_atomic64_read(&x) == 0);
}
END_TEST

START_TEST(test_atomic64_add)
{
    y_atomic64_t x = Y_ATOMIC64_INIT(0);
    test_threadpool_t *pool = test_threadpool_create(ATOMIC64_THREAD_COUNT);

    ck_assert(pool);

    thread_func_run_flag = false;
    y_barrier();

    test_threadpool_add_task_for_each(pool, thread_func_atomic64_add, &x);
    test_threadpool_add_task_for_each(pool, thread_func_atomic64_add, &x);

    y_barrier();
    thread_func_run_flag = true;

    test_threadpool_destroy(pool);

    ck_assert(
        y_atomic64_read(&x) ==
        ATOMIC64_THREAD_COUNT * ATOMIC64_OP_COUNT * ATOMIC64_ADD_STEP * 2);
}
END_TEST

START_TEST(test_atomic64_sub)
{
    y_atomic64_t x = Y_ATOMIC64_INIT(ATOMIC64_THREAD_COUNT * ATOMIC64_OP_COUNT *
                                 ATOMIC64_ADD_STEP * 2);
    test_threadpool_t *pool = test_threadpool_create(ATOMIC64_THREAD_COUNT);

    ck_assert(pool);

    thread_func_run_flag = false;
    y_barrier();

    test_threadpool_add_task_for_each(pool, thread_func_atomic64_sub, &x);
    test_threadpool_add_task_for_each(pool, thread_func_atomic64_sub, &x);

    y_barrier();
    thread_func_run_flag = true;

    test_threadpool_destroy(pool);

    ck_assert(y_atomic64_read(&x) == 0);
}
END_TEST

START_TEST(test_atomic64_dec_and_test)
{
    y_atomic64_t x[2];
    test_threadpool_t *pool = test_threadpool_create(ATOMIC64_THREAD_COUNT);

    ck_assert(pool);

    y_atomic64_set(&x[0], ATOMIC64_OP_COUNT);
    y_atomic64_set(&x[1], 0);

    thread_func_run_flag = false;
    y_barrier();

    test_threadpool_add_task_for_each(
        pool, thread_func_atomic64_dec_and_test, &x);

    y_barrier();
    thread_func_run_flag = true;

    test_threadpool_destroy(pool);

    ck_assert(y_atomic64_read(&x[1]) == 1);
}
END_TEST

START_TEST(test_atomic64_sub_and_test)
{
    y_atomic64_t x[2];
    test_threadpool_t *pool = test_threadpool_create(ATOMIC64_THREAD_COUNT);

    ck_assert(pool);

    y_atomic64_set(&x[0], ATOMIC64_OP_COUNT * ATOMIC64_ADD_STEP);
    y_atomic64_set(&x[1], 0);

    thread_func_run_flag = false;
    y_barrier();

    test_threadpool_add_task_for_each(
        pool, thread_func_atomic64_sub_and_test, &x);

    y_barrier();
    thread_func_run_flag = true;

    test_threadpool_destroy(pool);

    ck_assert(y_atomic64_read(&x[1]) == 1);
}
END_TEST

Suite* suite_atomic64(void)
{
    Suite *s;
    TCase *tc;

    s = suite_create("atomic64");

    tc = tcase_create("atomic64");
    tcase_add_test(tc, test_atomic64_inc);
    tcase_add_test(tc, test_atomic64_dec);
    tcase_add_test(tc, test_atomic64_add);
    tcase_add_test(tc, test_atomic64_sub);
    tcase_add_test(tc, test_atomic64_dec_and_test);
    tcase_add_test(tc, test_atomic64_sub_and_test);
    suite_add_tcase(s, tc);

    return s;
}
