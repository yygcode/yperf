/*
 * compiler.h: macro definition of compiler options
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

#ifndef __y_begin_extern_c
#ifdef __cplusplus
#define __y_begin_extern_c extern "C" {
#define __y_end_extern_c }
#else
#define __y_begin_extern_c
#define __y_end_extern_c
#endif
#endif

__y_begin_extern_c

#if !defined(__clang__) && !defined(__GNUC__)
#error "Unsupport compiler, only support gcc or clang."
#endif

#if !defined(__x86_64__)
#error "Unsupport platform, only test in x86_64 platform."
#endif

/* Always turn on SMP lock prefix */
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
#define y_noreturn              __attribute__((noreturn))
#endif

#ifndef __unreachable
#define __unreachable()         __builtin_unreachable()
#endif

#ifndef __printf
#define __printf(a, b)          __attribute__((__format__(printf, a, b)))
#endif

#ifndef __scanf
#define __scanf(a, b)           __attribute__((__format__(scanf, a, b)))
#endif

#ifndef __likely
#define __likely(x)             __builtin_expect(!!(x), 1)
#endif

#ifndef __unlikely
#define __unlikely(x)           __builtin_expect(!!(x), 0)
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

__y_end_extern_c
