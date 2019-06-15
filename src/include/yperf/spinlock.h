/*
 * spinlock.h: spin lock interface
 *
 * Copyright (C) 2012-2020 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * SPDX-License-Identifier: GPL-2.0-or-later<https://spdx.org/licenses/>
 * Also see https://www.gnu.org/licenses/gpl-2.0.en.html
 *
 * Visit https://github.com/yygcode/ycc for first version.
 */

#pragma once

#include <yperf/base.h>

__BEGIN_DECLS

typedef struct y_spinlock {
    volatile uint32_t lockval;
} y_spinlock_t;

#define Y_SPINLOCK_LOCKEDV   ((uint32_t)1)
#define Y_SPINLOCK_UNLOCKEDV ((uint32_t)0)

#define Y_SPINLOCK_INIT()               { .lockval = Y_SPINLOCK_UNLOCKEDV, }
#define Y_SPINLOCK_INIT_LOCKED()        { .lockval = Y_SPINLOCK_LOCKEDV, }

static __always_inline void y_spinlock_init(y_spinlock_t *lock)
{
    WRITE_ONCE(lock->lockval, Y_SPINLOCK_UNLOCKEDV);
}

static __always_inline void y_spinlock_init_locked(y_spinlock_t *lock)
{
    WRITE_ONCE(lock->lockval, Y_SPINLOCK_LOCKEDV);
}

static __always_inline bool y_spinlock_is_locked(y_spinlock_t *lock)
{
    return Y_SPINLOCK_LOCKEDV == READ_ONCE(lock->lockval);
}

static __always_inline bool y_spinlock_trylock(y_spinlock_t *lock)
{
    return Y_SPINLOCK_UNLOCKEDV == lock->lockval &&
        y_cmpxchg(&lock->lockval, Y_SPINLOCK_UNLOCKEDV, Y_SPINLOCK_LOCKEDV);
}

void __y_spinlock_lock_slow(y_spinlock_t *lock);

static __always_inline void y_spinlock_lock(y_spinlock_t *lock)
{
    if (y_spinlock_trylock(lock))
        return;

    return __y_spinlock_lock_slow(lock);
}

static __always_inline void y_spinlock_unlock(y_spinlock_t *lock)
{
    WRITE_ONCE(lock->lockval, Y_SPINLOCK_UNLOCKEDV);
}

__END_DECLS
