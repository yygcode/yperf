/*
 * barrier.h: memory barrier
 *
 * Copyright (C) 2012-2020 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * SPDX-License-Identifier: GPL-2.0-or-later<https://spdx.org/licenses/>
 * Also see https://www.gnu.org/licenses/gpl-2.0.en.html
 */

#pragma once

#include <yperf/x86_64/barrier.h>

#define y_barrier()             asm volatile("": : :"memory")
#define y_barrier_data(ptr)     asm volatile("": :"r"(ptr) :"memory")
#define y_dma_rmb()             y_barrier()
#define y_dma_wmb()             y_barrier()

#ifndef y_mb
#define y_mb()                  y_barrier()
#endif

#ifndef y_rmb
#define y_rmb()                 y_barrier()
#endif

#ifndef y_wmb
#define y_wmb()                 y_barrier()
#endif
