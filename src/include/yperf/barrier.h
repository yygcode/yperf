/*
 * barrier.h: memory barrier
 *
 * Copyright (C) 2012-2020 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * SPDX-License-Identifier: GPL-2.0-or-later<https://spdx.org/licenses/>
 * Also see https://www.gnu.org/licenses/gpl-2.0.en.html
 */

#pragma once

#include <yperf/compiler.h>

static __always_inline void y_mb()
{
    __atomic_thread_fence(__ATOMIC_ACQ_REL);
}

static __always_inline void y_rmb()
{
    __atomic_thread_fence(__ATOMIC_ACQUIRE);
}

static __always_inline void y_wmb()
{
    __atomic_thread_fence(__ATOMIC_RELEASE);
}

static __always_inline void y_barrier()
{
    y_mb();
}
