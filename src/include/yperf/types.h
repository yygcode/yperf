/*
 * types.h: common types
 *
 * Copyright (C) 2012-2020 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * Visit https://github.com/yygcode/ycc for first version.
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

#include <stdbool.h>
#include <stdint.h>

#include <yperf/compiler.h>

__y_begin_extern_c

typedef struct {
    volatile int32_t counter;
} y_atomic_t;

typedef struct {
    volatile int64_t counter;
} y_atomic64_t;

typedef struct {
    volatile uint32_t lockval;
} y_spinlock_t;

__y_end_extern_c
