/*
 * base.h: basic header files
 *
 * Copyright (C) 2012-2020 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * SPDX-License-Identifier: GPL-2.0-or-later<https://spdx.org/licenses/>
 * Also see https://www.gnu.org/licenses/gpl-2.0.en.html
 */

#pragma once

#include <yperf/barrier.h>
#include <yperf/compiler.h>
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

static __always_inline
void __y_read_once_size(const volatile void *p, void *res, size_t size)
{
    switch (size) {
    case 1: *(uint8_t *)res = *(volatile uint8_t *)p; break;
    case 2: *(uint16_t *)res = *(volatile uint16_t *)p; break;
    case 4: *(uint32_t *)res = *(volatile uint32_t *)p; break;
    case 8: *(uint64_t *)res = *(volatile uint64_t *)p; break;
    default:
        y_barrier();
        __builtin_memcpy((void *)res, (const void *)p, size);
        y_barrier();
    }
}

#ifndef READ_ONCE
#define READ_ONCE(x)                                    \
    ({                                                  \
        union { typeof(x) __val; char __c[1]; } __u;    \
        __y_read_once_size(&(x), __u.__c, sizeof(x));   \
	__u.__val;                                      \
    })
#endif

static __always_inline
void __y_write_once_size(volatile void *p, const void *res, size_t size)
{
    switch (size) {
    case 1: *(volatile uint8_t *)p = *(const uint8_t *)res; break;
    case 2: *(volatile uint16_t *)p = *(const uint16_t *)res; break;
    case 4: *(volatile uint32_t *)p = *(const uint32_t *)res; break;
    case 8: *(volatile uint64_t *)p = *(const uint64_t *)res; break;
    default:
        y_barrier();
        __builtin_memcpy((void *)p, res, size);
        y_barrier();
    }
}

#ifndef WRITE_ONCE
#define WRITE_ONCE(x, val)                              \
    ({                                                  \
        union { typeof(x) __val; char __c[1]; } __u = { \
            .__val = (typeof(x)) (val)                  \
        };                                              \
        __y_write_once_size(&(x), __u.__c, sizeof(x));  \
        __u.__val;                                      \
    })
#endif

__END_DECLS
