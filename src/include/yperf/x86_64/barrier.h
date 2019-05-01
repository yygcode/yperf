/*
 * barrier.h: Intel x86_64 memory barrier
 *
 * Copyright (C) 2012-2020 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * SPDX-License-Identifier: GPL-2.0-or-later<https://spdx.org/licenses/>
 * Also see https://www.gnu.org/licenses/gpl-2.0.en.html
 */

#pragma once

#include <yperf/compiler.h>

__BEGIN_DECLS

#if defined(__x86_64__)

#define y_mb()                  asm volatile("mfence":::"memory")
#define y_rmb()                 asm volatile("lfence":::"memory")
#define y_wmb()                 asm volatile("sfence" ::: "memory")

#endif /* end __x86_64__ */

__END_DECLS
