/*
 * atomic.h: 32-bits atomic in SMP system
 *
 * Copyright (C) 2012-2020 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * Visit https://github.com/yygcode/ycc for first version.
 *
 * Assembly Tech Visit http://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
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

#pragma once

#include <yperf/types.h>

__y_begin_extern_c

#define Y_ATOMIC_INIT(i)        { (i) }

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
    __asm__ volatile(Y_LOCK_PREFIX "incl %0"
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
    __asm__ volatile(Y_LOCK_PREFIX
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
    __asm__ volatile(Y_LOCK_PREFIX
                     "subl %2, %0\n\t"
                     "sete %1\n\t"
                     : "+m"(v->counter), "=qm"(c)
                     : "ir"(i)
                     : "memory");
    return c != 0;
}

__y_end_extern_c
