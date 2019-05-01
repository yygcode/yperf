/*
 * spinlock.c: spin lock implementation
 *
 * Copyright (C) 2012-2020 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * SPDX-License-Identifier: GPL-2.0-or-later<https://spdx.org/licenses/>
 * Also see https://www.gnu.org/licenses/gpl-2.0.en.html
 */

#include <sched.h>

#include <yperf/spinlock.h>

void __y_spinlock_lock_slow(y_spinlock_t *lock)
{
    for (;;)
    {
        for (uint32_t i = 0; i < 512; ++i)
        {
            if (y_spinlock_trylock(lock))
                return;

            /* pause is alias of asm(".byte 0xf3, 0x90") */
            asm("pause");
        }

        /* relinquish cpu */
        sched_yield();
    }
}
