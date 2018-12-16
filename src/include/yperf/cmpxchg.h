/*
 * cmpxchg.h: Atomic compare and change
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

#include <yperf/types.h>

__y_begin_extern_c

static __always_inline bool y_cmpxchg(volatile uint32_t *_ptr,
                                      uint32_t _old, uint32_t _new)
{
    uint32_t _ret;

    __asm__ __volatile__(Y_LOCK_PREFIX
                         "cmpxchgl %2, %1"
                         : "=a" (_ret), "+m" (*_ptr)
                         : "r" (_new), "0" (_old)
                         : "memory");
    return _ret == _old;
}

static __always_inline bool y_cmpxchg64(volatile uint64_t *_ptr,
                                        uint64_t _old, uint64_t _new)
{
    uint64_t _ret;

    __asm__ __volatile__(Y_LOCK_PREFIX
                         "cmpxchgq %2, %1"
                         : "=a" (_ret), "+m" (*_ptr)
                         : "r" (_new), "0" (_old)
                         : "memory");
    return _ret == _old;
}

__y_end_extern_c
