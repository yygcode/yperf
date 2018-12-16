#include <check.h>
#include <pthread.h>
#include <stdio.h>

#include <yperf/atomic.h>
#include <yperf/barrier.h>

#include "threadpool.h"

/* Create a basic thread pool to test atomic operation */
enum
{
    ATOMIC_OP_COUNT = 1000 * 100,
    ATOMIC_THREAD_COUNT = 20,
    ATOMIC_ADD_STEP = 11,
};

static volatile bool thread_func_run_flag = false;
static void thread_func_atomic_inc(void *arg)
{
    y_atomic_t *x = (y_atomic_t*)arg;

    while (!thread_func_run_flag); /* busy wait */

    for (size_t i = 0; i < ATOMIC_OP_COUNT; ++i) {
        y_atomic_inc(x);
    }
}

static void thread_func_atomic_dec(void *arg)
{
    y_atomic_t *x = (y_atomic_t*)arg;

    while (!thread_func_run_flag); /* busy wait */

    for (size_t i = 0; i < ATOMIC_OP_COUNT; ++i) {
        y_atomic_dec(x);
    }
}

static void thread_func_atomic_add(void *arg)
{
    y_atomic_t *x = (y_atomic_t*)arg;

    while (!thread_func_run_flag); /* busy wait */

    for (size_t i = 0; i < ATOMIC_OP_COUNT; ++i) {
        y_atomic_add(x, ATOMIC_ADD_STEP);
    }
}

static void thread_func_atomic_sub(void *arg)
{
    y_atomic_t *x = (y_atomic_t*)arg;

    while (!thread_func_run_flag); /* busy wait */

    for (size_t i = 0; i < ATOMIC_OP_COUNT; ++i) {
        y_atomic_sub(x, ATOMIC_ADD_STEP);
    }
}

static void thread_func_atomic_dec_and_test(void *arg)
{
    y_atomic_t *x = (y_atomic_t*)arg;
    y_atomic_t *y = x + 1;

    while (!thread_func_run_flag); /* busy wait */

    for (size_t i = 0; i < ATOMIC_OP_COUNT; ++i) {
        if (y_atomic_dec_and_test(x)) {
            y_atomic_inc(y);
        }
    }
}

static void thread_func_atomic_sub_and_test(void *arg)
{
    y_atomic_t *x = (y_atomic_t*)arg;
    y_atomic_t *y = x + 1;

    while (!thread_func_run_flag); /* busy wait */

    for (size_t i = 0; i < ATOMIC_OP_COUNT; ++i) {
        if (y_atomic_sub_and_test(x, ATOMIC_ADD_STEP)) {
            y_atomic_inc(y);
        }
    }
}

START_TEST(test_atomic_inc)
{
    y_atomic_t x = Y_ATOMIC_INIT(0);
    test_threadpool_t *pool = test_threadpool_create(ATOMIC_THREAD_COUNT);

    ck_assert(pool);

    thread_func_run_flag = false;
    y_barrier();

    test_threadpool_add_task_for_each(pool, thread_func_atomic_inc, &x);
    test_threadpool_add_task_for_each(pool, thread_func_atomic_inc, &x);

    y_barrier();
    thread_func_run_flag = true;

    test_threadpool_destroy(pool);

    ck_assert(y_atomic_read(&x) == ATOMIC_THREAD_COUNT * ATOMIC_OP_COUNT * 2);
}
END_TEST

START_TEST(test_atomic_dec)
{
    y_atomic_t x = Y_ATOMIC_INIT(ATOMIC_THREAD_COUNT * ATOMIC_OP_COUNT * 2);
    test_threadpool_t *pool = test_threadpool_create(ATOMIC_THREAD_COUNT);

    ck_assert(pool);

    thread_func_run_flag = false;
    y_barrier();

    test_threadpool_add_task_for_each(pool, thread_func_atomic_dec, &x);
    test_threadpool_add_task_for_each(pool, thread_func_atomic_dec, &x);

    y_barrier();
    thread_func_run_flag = true;

    test_threadpool_destroy(pool);

    ck_assert(y_atomic_read(&x) == 0);
}
END_TEST

START_TEST(test_atomic_add)
{
    y_atomic_t x = Y_ATOMIC_INIT(0);
    test_threadpool_t *pool = test_threadpool_create(ATOMIC_THREAD_COUNT);

    ck_assert(pool);

    thread_func_run_flag = false;
    y_barrier();

    test_threadpool_add_task_for_each(pool, thread_func_atomic_add, &x);
    test_threadpool_add_task_for_each(pool, thread_func_atomic_add, &x);

    y_barrier();
    thread_func_run_flag = true;

    test_threadpool_destroy(pool);

    ck_assert(y_atomic_read(&x) ==
              ATOMIC_THREAD_COUNT * ATOMIC_OP_COUNT * ATOMIC_ADD_STEP * 2);
}
END_TEST

START_TEST(test_atomic_sub)
{
    y_atomic_t x = Y_ATOMIC_INIT(ATOMIC_THREAD_COUNT * ATOMIC_OP_COUNT *
                                 ATOMIC_ADD_STEP * 2);
    test_threadpool_t *pool = test_threadpool_create(ATOMIC_THREAD_COUNT);

    ck_assert(pool);

    thread_func_run_flag = false;
    y_barrier();

    test_threadpool_add_task_for_each(pool, thread_func_atomic_sub, &x);
    test_threadpool_add_task_for_each(pool, thread_func_atomic_sub, &x);

    y_barrier();
    thread_func_run_flag = true;

    test_threadpool_destroy(pool);

    ck_assert(y_atomic_read(&x) == 0);
}
END_TEST

START_TEST(test_atomic_dec_and_test)
{
    y_atomic_t x[2];
    test_threadpool_t *pool = test_threadpool_create(ATOMIC_THREAD_COUNT);

    ck_assert(pool);

    y_atomic_set(&x[0], ATOMIC_OP_COUNT);
    y_atomic_set(&x[1], 0);

    thread_func_run_flag = false;
    y_barrier();

    test_threadpool_add_task_for_each(
        pool, thread_func_atomic_dec_and_test, &x);

    y_barrier();
    thread_func_run_flag = true;

    test_threadpool_destroy(pool);

    ck_assert(y_atomic_read(&x[1]) == 1);
}
END_TEST

START_TEST(test_atomic_sub_and_test)
{
    y_atomic_t x[2];
    test_threadpool_t *pool = test_threadpool_create(ATOMIC_THREAD_COUNT);

    ck_assert(pool);

    y_atomic_set(&x[0], ATOMIC_OP_COUNT * ATOMIC_ADD_STEP);
    y_atomic_set(&x[1], 0);

    thread_func_run_flag = false;
    y_barrier();

    test_threadpool_add_task_for_each(
        pool, thread_func_atomic_sub_and_test, &x);

    y_barrier();
    thread_func_run_flag = true;

    test_threadpool_destroy(pool);

    ck_assert(y_atomic_read(&x[1]) == 1);
}
END_TEST

Suite* suite_atomic(void)
{
    Suite *s;
    TCase *tc;

    s = suite_create("atomic");

    tc = tcase_create("atomic");
    tcase_add_test(tc, test_atomic_inc);
    tcase_add_test(tc, test_atomic_dec);
    tcase_add_test(tc, test_atomic_add);
    tcase_add_test(tc, test_atomic_sub);
    tcase_add_test(tc, test_atomic_dec_and_test);
    tcase_add_test(tc, test_atomic_sub_and_test);
    suite_add_tcase(s, tc);

    return s;
}
