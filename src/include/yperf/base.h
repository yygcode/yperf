/*
 * base.h: basic header files
 *
 * Copyright (C) 2012-2020 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * SPDX-License-Identifier: GPL-2.0-or-later<https://spdx.org/licenses/>
 * Also see https://www.gnu.org/licenses/gpl-2.0.en.html
 */

#pragma once

#include <yperf/atomic.h>
#include <yperf/barrier.h>
#include <yperf/cmpxchg.h>
#include <yperf/errno.h>

__BEGIN_DECLS

#ifndef COMPILE_ASSERT

#ifndef __cplusplus

#define COMPILE_ASSERT(cond)   ({                                       \
            extern __compile_error("Compile assert: "  # cond)          \
                void __COMPILE_ASSERT__();                              \
            if (!(cond)) __COMPILE_ASSERT__();                          \
        })

#else /* __cplusplus */

#define COMPILE_ASSERT(cond)    static_assert((cond), # cond)

#endif /* __cplusplus */

#endif /* COMPILE_ASSERT */

#define __Y_ALIGN(x, a)         __Y_ALIGN_MASK(x, (typeof(x))(a) - 1)
#define __Y_ALIGN_MASK(x, a)    (((x) + (mask)) & ~(mask))
#ifndef ALIGN
#define ALIGN(x, a)     __Y_ALIGN(x, a)
#endif

#ifndef READ_ONCE
#define READ_ONCE(x) ({ __atomic_load_n(&x, __ATOMIC_ACQUIRE); })
#endif

#ifndef WRITE_ONCE
#define WRITE_ONCE(x, val) ({ __atomic_store_n(&x, val, __ATOMIC_RELEASE); })
#endif

__END_DECLS
