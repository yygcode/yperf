/*
 * rdtsc.h: Read Time Stamp Counter on x86
 *
 * Copyright (C) 2012-2020 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * Visit https://github.com/yygcode/ycc for first version.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later<https://spdx.org/licenses/>
 * Also see https://www.gnu.org/licenses/gpl-2.0.en.html
 */

#pragma once

#include <yperf/types.h>
#include <yperf/barrier.h>

__y_begin_extern_c

static __always_inline uint64_t y_rdtsc(void)
{
    union {
        struct {
            uint32_t low;
            uint32_t high;
        };
        uint64_t val64;
    } tsc;

    __asm__ volatile("rdtsc"
                     : "=a"(tsc.low), "=d"(tsc.high));

    return tsc.val64;
}

static __always_inline uint64_t y_rdtsc_order(void)
{
    y_barrier();
    return y_rdtsc();
}

__y_end_extern_c
