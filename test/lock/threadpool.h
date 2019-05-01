/*
 * threadpool.h: simple thread pool for lock test
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

/*
 * Important Notice:
 * DO NOT USE for product, only test using, performance bad.
 */

#pragma once

#include <yperf/base.h>

__BEGIN_DECLS

typedef struct test_threadpool test_threadpool_t;

test_threadpool_t* test_threadpool_create(size_t n);
void test_threadpool_destroy(test_threadpool_t *pool);

void test_threadpool_add_task(
    test_threadpool_t *pool, size_t thread_idx, void (*func)(void*), void *arg);

void test_threadpool_add_task_for_each(
    test_threadpool_t *pool, void (*func)(void*), void *arg);

bool test_threadpool_empty(test_threadpool_t *pool, size_t thread_idx);
bool test_threadpool_empty_all(test_threadpool_t *pool);

void test_threadpool_wait(test_threadpool_t *pool, size_t thread_idx);
void test_threadpool_wait_all(test_threadpool_t *pool);

__END_DECLS
