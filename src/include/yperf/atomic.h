/*
 * atomic.h: Atomic and atomic64
 *
 * Copyright (C) 2012-2020 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * SPDX-License-Identifier: GPL-2.0-or-later<https://spdx.org/licenses/>
 * Also see https://www.gnu.org/licenses/gpl-2.0.en.html
 *
 * The file includes barrier, atomic, and cmpxchg
 */

#pragma once

#include <yperf/compiler.h>

__BEGIN_DECLS

typedef struct {
    volatile int32_t counter;
} y_atomic_t;

typedef struct {
    volatile int64_t counter;
} y_atomic64_t;

#define Y_ATOMIC_INIT(i)        { (i) }
#define Y_ATOMIC64_INIT(i)      { ((int64_t)i) }

static __always_inline void y_atomic_set(y_atomic_t *v, int32_t i)
{
    __atomic_store_n(&v->counter, i, __ATOMIC_RELEASE);
}

static __always_inline int y_atomic_read(const y_atomic_t *v)
{
    return __atomic_load_n(&v->counter, __ATOMIC_ACQUIRE);
}

static __always_inline void y_atomic_add(y_atomic_t *v, int32_t i)
{
    (void)__atomic_add_fetch(&v->counter, i, __ATOMIC_RELAXED);
}

static __always_inline void y_atomic_sub(y_atomic_t *v, int32_t i)
{
    (void)__atomic_sub_fetch(&v->counter, i, __ATOMIC_RELAXED);
}

static __always_inline void y_atomic_inc(y_atomic_t *v)
{
    y_atomic_add(v, 1);
}

static __always_inline void y_atomic_dec(y_atomic_t *v)
{
    y_atomic_sub(v, 1);
}

static __always_inline bool y_atomic_add_and_test(y_atomic_t *v, int32_t i)
{
    return 0 == __atomic_add_fetch(&v->counter, i, __ATOMIC_ACQ_REL);
}

static __always_inline bool y_atomic_sub_and_test(y_atomic_t *v, int32_t i)
{
    return 0 == __atomic_sub_fetch(&v->counter, i, __ATOMIC_ACQ_REL);
}

static __always_inline bool y_atomic_inc_and_test(y_atomic_t *v)
{
    return y_atomic_add_and_test(v, 1);
}

static __always_inline bool y_atomic_dec_and_test(y_atomic_t *v)
{
    return y_atomic_sub_and_test(v, 1);
}

static __always_inline void y_atomic64_set(y_atomic64_t *v, int64_t i)
{
    __atomic_store_n(&v->counter, i, __ATOMIC_RELEASE);
}

static __always_inline long y_atomic64_read(const y_atomic64_t *v)
{
    return __atomic_load_n(&v->counter, __ATOMIC_ACQUIRE);
}

static __always_inline void y_atomic64_add(y_atomic64_t *v, int64_t i)
{
    (void)__atomic_add_fetch(&v->counter, i, __ATOMIC_RELAXED);
}

static __always_inline void y_atomic64_sub(y_atomic64_t *v, int64_t i)
{
    (void)__atomic_sub_fetch(&v->counter, i, __ATOMIC_RELAXED);
}

static __always_inline void y_atomic64_inc(y_atomic64_t *v)
{
    y_atomic64_add(v, 1);
}

static __always_inline void y_atomic64_dec(y_atomic64_t *v)
{
    y_atomic64_sub(v, 1);
}

static __always_inline bool y_atomic64_add_and_test(y_atomic64_t *v, int64_t i)
{
    return 0 == __atomic_add_fetch(&v->counter, i, __ATOMIC_ACQ_REL);
}

static __always_inline bool y_atomic64_sub_and_test(y_atomic64_t *v, int64_t i)
{
    return 0 == __atomic_sub_fetch(&v->counter, i, __ATOMIC_ACQ_REL);
}

static __always_inline bool y_atomic64_inc_and_test(y_atomic64_t *v)
{
    return y_atomic64_add_and_test(v, 1);
}

static __always_inline bool y_atomic64_dec_and_test(y_atomic64_t *v)
{
    return y_atomic64_sub_and_test(v, 1);
}

__END_DECLS
