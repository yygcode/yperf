/*
 * barrier.h: memory barrier in SMP system
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

#define y_barrier()             __asm__ volatile("": : :"memory")
#define y_barrier_data(ptr)     __asm__ volatile("": :"r"(ptr) :"memory")
#define y_mb()                  __asm__ volatile("mfence":::"memory")
#define y_rmb()                 __asm__ volatile("lfence":::"memory")
#define y_wmb()                 __asm__ volatile("sfence" ::: "memory")

#define y_dma_rmb()             y_barrier()
#define y_dma_wmb()             y_barrier()

__y_end_extern_c
