/*
 * compiler.h: compiler relative definitions
 *
 * Copyright (C) 2012-2020 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * SPDX-License-Identifier: GPL-2.0-or-later<https://spdx.org/licenses/>
 * Also see https://www.gnu.org/licenses/gpl-2.0.en.html
 */

#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#include <sys/cdefs.h>

#ifndef __BEGIN_DECLS
#ifdef __cplusplus
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#else
#define __BEGIN_DECLS
#define __END_DECLS
#endif
#endif

__BEGIN_DECLS

#if !defined(__clang__) && !defined(__GNUC__)
#error "Unsupport compiler, only support gcc or clang."
#endif

#if !defined(__x86_64__)
#error "Unsupport platform, only test in x86_64 platform."
#endif

/* Y_LOCK_PREFIX can be predefined only for YPERF_TEST */
#if defined(Y_LOCK_PREFIX) && !defined(YPERF_TEST)
#error "Please define YPERF_TEST for test, or a predefined symbol conflicts ?"
#endif

#ifndef Y_LOCK_PREFIX
#define Y_LOCK_PREFIX           "\n\tlock; "
#endif

#ifndef __aligned
#define __aligned(x)            __attribute__((__aligned__(x)))
#endif

#ifndef __always_inline
#define __always_inline         inline __attribute__((__always_inline__))
#endif

#ifndef __noinline
#define __noinline              __attribute__((noinline))
#endif

#ifndef __noreturn
#define __noreturn              __attribute__((noreturn))
#endif

#ifndef __printf
#define __printf(a, b)          __attribute__((__format__(printf, a, b)))
#endif

#ifndef __scanf
#define __scanf(a, b)           __attribute__((__format__(scanf, a, b)))
#endif

#ifndef __nonnull
#define __nonnull(params)       __attribute__ ((__nonnull__ params))
#endif

#ifndef __pure
#define __pure                  __attribute__ ((__pure__))
#endif

#ifndef __const
#define __const                 __attribute__ ((__const__))
#endif

#ifndef __unused
#define __unused                __attribute__((__unused__))
#endif

#ifndef __used
#define __used                  __attribute__((__used__))
#endif

#ifndef __must_check
#define __must_check            __attribute__((__warn_unused_result__))
#endif

#ifndef __malloc
#define __malloc		__attribute__((__malloc__))
#endif

#ifndef __cold
#define __cold                  __attribute__((__cold__))
#endif

#ifndef __hot
#define __hot                   __attribute__((__hot__))
#endif

#ifndef __unreachable
#define __unreachable()         __builtin_unreachable()
#endif

#ifndef __compile_warning
#define __compile_warning(msg)  __attribute__((warning(msg)))
#endif

#ifndef __compile_error
#define __compile_error(msg)    __attribute__((error(msg)))
#endif

#ifndef __inline
#define __inline                inline
#endif

#ifndef __likely
#define __likely(x)             __builtin_expect(!!(x), 1)
#endif

#ifndef __unlikely
#define __unlikely(x)           __builtin_expect(!!(x), 0)
#endif

#ifndef likely
#define likely __likely
#endif

#ifndef unlikely
#define unlikely __unlikely
#endif

#ifndef __uninitialized_var
#define __uninitialized_var(x)  x = *(&(x))
#endif

#ifndef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE, MEMBER)  __compiler_offsetof(TYPE,MEMBER)
#else
#define offsetof(TYPE, MEMBER)  ((size_t) &((TYPE*)0)->MEMBER)
#endif
#endif

#ifndef container_of
#define container_of(ptr, type, member) \
    ((type*)((char*)ptr - offsetof(type, member)))
#endif

#ifndef __unused
#define __unused(x)     ((void)(var))
#endif

#ifdef __cplusplus
#define typeof decltype
#endif

__END_DECLS
