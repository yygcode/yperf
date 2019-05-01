/*
 * atomic.h: 32-bits atomic in SMP system
 *
 * Copyright (C) 2012-2020 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * SPDX-License-Identifier: GPL-2.0-or-later<https://spdx.org/licenses/>
 * Also see https://www.gnu.org/licenses/gpl-2.0.en.html
 */

#pragma once

#include <yperf/base.h>

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
    v->counter = i;
}

static __always_inline int y_atomic_read(const y_atomic_t *v)
{
    return v->counter;
}

static __always_inline void y_atomic_inc(y_atomic_t *v)
{
    asm volatile(Y_LOCK_PREFIX "incl %0"
                 : "+m" (v->counter));
}

static __always_inline void y_atomic_dec(y_atomic_t *v)
{
    __asm__ volatile(Y_LOCK_PREFIX "decl %0"
                     : "+m" (v->counter));
}

static __always_inline bool y_atomic_dec_and_test(y_atomic_t *v)
{
    char c;
    asm volatile(Y_LOCK_PREFIX
                 "decl %0\n\t"
                 "sete %1\n\t"
                 : "+m"(v->counter), "=qm"(c)
                 :
                 : "memory");
    return c != 0;
}

static __always_inline void y_atomic_add(y_atomic_t *v, int32_t i)
{
    asm volatile(Y_LOCK_PREFIX "addl %1, %0"
                 : "+m" (v->counter)
                 : "ir" (i));
}

static __always_inline void y_atomic_sub(y_atomic_t *v, int32_t i)
{
    asm volatile(Y_LOCK_PREFIX "subl %1, %0"
                 : "+m" (v->counter)
                 : "ir" (i));
}

static __always_inline bool y_atomic_sub_and_test(y_atomic_t *v, int32_t i)
{
    char c;
    asm volatile(Y_LOCK_PREFIX
                 "subl %2, %0\n\t"
                 "sete %1\n\t"
                 : "+m"(v->counter), "=qm"(c)
                 : "ir"(i)
                 : "memory");
    return c != 0;
}


static __always_inline void y_atomic64_set(y_atomic64_t *v, int64_t i)
{
    v->counter = i;
}

static __always_inline int64_t y_atomic64_read(const y_atomic64_t *v)
{
    return v->counter;
}

static __always_inline void y_atomic64_inc(y_atomic64_t *v)
{
    asm volatile(Y_LOCK_PREFIX "incq %0"
                 : "+m" (v->counter));
}

static __always_inline void y_atomic64_dec(y_atomic64_t *v)
{
    asm volatile(Y_LOCK_PREFIX "decq %0"
                 : "+m" (v->counter));
}

static __always_inline bool y_atomic64_dec_and_test(y_atomic64_t *v)
{
    char c;
    asm volatile(Y_LOCK_PREFIX
                 "decq %0\n\t"
                 "sete %1\n\t"
                 : "+m"(v->counter), "=qm"(c)
                 :
                 : "memory");
    return c != 0;
}

static __always_inline void y_atomic64_add(y_atomic64_t *v, int64_t i)
{
    asm volatile(Y_LOCK_PREFIX "addq %1, %0"
                 : "+m" (v->counter)
                 : "ir" (i));
}

static __always_inline void y_atomic64_sub(y_atomic64_t *v, int64_t i)
{
    asm volatile(Y_LOCK_PREFIX "subq %1, %0"
                 : "+m" (v->counter)
                 : "ir" (i));
}

static __always_inline bool y_atomic64_sub_and_test(y_atomic64_t *v, int64_t i)
{
    char c;
    asm volatile(Y_LOCK_PREFIX
                 "subq %2, %0\n\t"
                 "sete %1\n\t"
                 : "+m"(v->counter), "=qm"(c)
                 : "ir"(i)
                 : "memory");
    return c != 0;
}

__END_DECLS
