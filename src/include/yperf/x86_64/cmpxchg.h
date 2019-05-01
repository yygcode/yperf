/*
 * cmpxchg.h: Intel x86_64 Atomic compare and change
 *
 * Copyright (C) 2012-2020 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * SPDX-License-Identifier: GPL-2.0-or-later<https://spdx.org/licenses/>
 * Also see https://www.gnu.org/licenses/gpl-2.0.en.html
 *
 * xcmpchg logic:
 * lock
 *   result = *_ptr;
 *   if (result == _old)
 *     *_ptr = _new;
 *   return result == _old;
 * unlock
 *
 */

#pragma once

#include <yperf/base.h>

__BEGIN_DECLS

#if defined(__x86_64__)

static __always_inline bool y_cmpxchg(
    volatile uint32_t *_ptr, uint32_t _old, uint32_t _new)
{
    uint32_t _ret;

    asm volatile(Y_LOCK_PREFIX
                 "cmpxchgl %2, %1"
                 : "=a" (_ret), "+m" (*_ptr)
                 : "r" (_new), "0" (_old)
                 : "memory");
    return _ret == _old;
}

static __always_inline bool y_cmpxchg64(
    volatile uint64_t *_ptr, uint64_t _old, uint64_t _new)
{
    uint64_t _ret;

    asm volatile(Y_LOCK_PREFIX
                 "cmpxchgq %2, %1"
                 : "=a" (_ret), "+m" (*_ptr)
                 : "r" (_new), "0" (_old)
                 : "memory");
    return _ret == _old;
}

static __always_inline uint32_t y_xchg(volatile uint32_t *p, uint32_t val)
{
    uint32_t ret = val;

    /* LOCK PREFIX implied */
    asm volatile("xchgl %0, %1"
                 : "+r" (ret), "+m" (*p)
                 :
                 : "memory", "cc");
    return ret;
}

static __always_inline uint64_t y_xchg64(volatile uint64_t *p, uint64_t val)
{
    uint64_t ret = val;

    /* LOCK PREFIX implied */
    asm volatile("xchgq %0, %1"
                 : "+r" (ret), "+m" (*p)
                 :
                 : "memory", "cc");
    return ret;
}

#endif /* end __x86_64__ */

__END_DECLS
