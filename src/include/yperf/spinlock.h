/*
 * spinlock.h: spin lock interface
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
 *
 * You should always use pthread_mutex instead of spinlock unless
 * you know exactly what you are doing.
 *
 */

#pragma once

#include <yperf/barrier.h>
#include <yperf/cmpxchg.h>
#include <yperf/types.h>

__y_begin_extern_c

#define Y_SPINLOCK_VAL_LOCKED   1
#define Y_SPINLOCK_VAL_UNLOCKED 0

#define Y_SPINLOCK_INIT()               { .lockval = Y_SPINLOCK_VAL_UNLOCKED, }
#define Y_SPINLOCK_INIT_LOCKED()        { .lockval = Y_SPINLOCK_VAL_LOCKED, }

static __always_inline void y_spinlock_init(y_spinlock_t *lock)
{
    lock->lockval = Y_SPINLOCK_VAL_UNLOCKED;
}

static __always_inline void y_spinlock_init_locked(y_spinlock_t *lock)
{
    lock->lockval = Y_SPINLOCK_VAL_LOCKED;
}

static __always_inline bool y_spinlock_is_locked(y_spinlock_t *lock)
{
    return lock->lockval == Y_SPINLOCK_VAL_LOCKED;
}

static __always_inline void y_spinlock_lock(y_spinlock_t *lock)
{
    while (!y_cmpxchg(&lock->lockval,
                      Y_SPINLOCK_VAL_UNLOCKED, Y_SPINLOCK_VAL_LOCKED));
}

static __always_inline void y_spinlock_unlock(y_spinlock_t *lock)
{
    y_barrier();
    lock->lockval = Y_SPINLOCK_VAL_UNLOCKED;
}

static __always_inline bool y_spinlock_trylock(y_spinlock_t *lock)
{
    return y_cmpxchg(&lock->lockval,
                     Y_SPINLOCK_VAL_UNLOCKED, Y_SPINLOCK_VAL_LOCKED);
}

__y_end_extern_c
