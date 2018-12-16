/*
 * threadpool.c: simple thread pool for lock test
 *
 * Copyright (C) 2012-2020 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * Visit https://github.com/yygcode/ycc for first version.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING, if not see
 * <http://www.gnu.org/licenses/>.
 */

#include <malloc.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "threadpool.h"

struct thread_task
{
    void (*func)(void*);
    void* arg;
    struct thread_task *next;
};

struct thread_context
{
    size_t idx;
    pthread_t thread;
    bool stopped;  // stopped to receive new task
    bool running;  // task executing
    pthread_mutex_t lock;
    struct thread_task *task;
    struct thread_task **plast_task;
};

struct test_threadpool
{
    size_t n;
    struct thread_context context[0];
};

static void* __thread_func(void *arg);
static void __thread_stop(test_threadpool_t *pool, size_t n);

test_threadpool_t* test_threadpool_create(size_t n)
{
    size_t i;
    test_threadpool_t *pool =
        malloc(sizeof(*pool) + sizeof(struct thread_context)*n);

    if (!pool)
        return NULL;

    for (i = 0; i < n; ++i)
    {
        struct thread_context *ctx = pool->context + i;

        /* init before thread create */
        ctx->idx = i;
        ctx->stopped = false;
        ctx->running = false;
        pthread_mutex_init(&ctx->lock, NULL);
        ctx->task = NULL;
        ctx->plast_task = &ctx->task;

        int r = pthread_create(&ctx->thread, NULL, __thread_func, (void*)ctx);
        if (r != 0) {
            pthread_mutex_destroy(&ctx->lock);
            break;
        }
    }

    pool->n = i;

    if (i != n && i != 0) {
        /* failure break */
        __thread_stop(pool, i - 1);

        free(pool);
        pool = NULL;
    }

    return pool;
}

void test_threadpool_destroy(test_threadpool_t *pool)
{
    register size_t n = pool->n;

    /* disable new task */
    for (size_t i = 0; i < n; ++i) {
        pool->context[i].stopped = true;
    }

    /* wait all pending task finish */
    for (size_t i = 0; i < n; ++i) {
        struct thread_context *ctx = pool->context + i;
        pthread_mutex_lock(&ctx->lock);
        while (ctx->task) {
            pthread_mutex_unlock(&ctx->lock);
            usleep(1);
            pthread_mutex_lock(&ctx->lock);
        }
        pthread_mutex_unlock(&ctx->lock);
    }

    __thread_stop(pool, n);
    free(pool);
}

void test_threadpool_add_task(
    test_threadpool_t *pool, size_t thread_idx, void (*func)(void*), void * arg)
{
    struct thread_context *ctx = pool->context + thread_idx;
    struct thread_task *task = malloc(sizeof(*task));

    /* ignore malloc failure check */
    task->arg = arg;
    task->func = func;
    task->next = NULL;

    pthread_mutex_lock(&ctx->lock);
    *ctx->plast_task = task;
    ctx->plast_task = &task->next;
    pthread_mutex_unlock(&ctx->lock);
}

void test_threadpool_add_task_for_each(
    test_threadpool_t *pool, void (*func)(void*), void * arg)
{
    size_t n = pool->n;

    for (size_t i = 0; i < n; ++i) {
        test_threadpool_add_task(pool, i, func, arg);
    }
}

bool test_threadpool_empty(test_threadpool_t *pool, size_t thread_idx)
{
    bool ret;
    struct thread_context *ctx = pool->context + thread_idx;

    pthread_mutex_lock(&ctx->lock);
    ret = (!ctx->running && (ctx->task == NULL));
    pthread_mutex_unlock(&ctx->lock);

    return ret;
}

bool test_threadpool_empty_all(test_threadpool_t *pool)
{
    size_t n = pool->n;

    for (size_t i = 0; i < n; ++i) {
        if (!test_threadpool_empty(pool, i)) {
            return false;
        }
    }

    return true;
}

void test_threadpool_wait(test_threadpool_t *pool, size_t thread_idx)
{
    while (!test_threadpool_empty(pool, thread_idx))
        usleep(1);
}

void test_threadpool_wait_all(test_threadpool_t *pool)
{
    while (!test_threadpool_empty_all(pool))
        usleep(1);
}

static void* __thread_func(void *arg)
{
    struct thread_context *ctx = arg;

    while (true)
    {
        struct thread_task *task;
        pthread_mutex_lock(&ctx->lock);
        while (!ctx->task) {
            ctx->running = false;
            pthread_mutex_unlock(&ctx->lock);
            usleep(1);
            pthread_mutex_lock(&ctx->lock);
        }
        ctx->running = true;
        task = ctx->task;
        ctx->task = task->next;
        if (!ctx->task)
            ctx->plast_task = &ctx->task;
        pthread_mutex_unlock(&ctx->lock);

        task->func(task->arg);
        free(task);
    }

    return NULL;
}

static void __thread_stop(test_threadpool_t* pool, size_t n)
{
    size_t i;

    for (i = 0; i < n; ++i)
    {
        struct thread_context *ctx = pool->context + i;
        pthread_cancel(ctx->thread);
        pthread_mutex_destroy(&ctx->lock);
    }

    for (i = 0; i < n; ++i)
    {
        pthread_join(pool->context[i].thread, NULL);
    }
}
