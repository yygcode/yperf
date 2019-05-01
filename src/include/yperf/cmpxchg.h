/*
 * x86_64.h: Intel x86_64 platform code
 *
 * Copyright (C) 2012-2020 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * SPDX-License-Identifier: GPL-2.0-or-later<https://spdx.org/licenses/>
 * Also see https://www.gnu.org/licenses/gpl-2.0.en.html
 *
 * Assembly Tech Visit http://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
 */

#pragma once

#include <yperf/compiler.h>
#include <yperf/x86_64/cmpxchg.h>

/*
 * xchg logic:
 * lock
 *   ret = *ptr;
 *   *ptr = val;
 * unlock
 * return ret;
 *
 * xcmpchg logic:
 * lock
 *   result = *_ptr;
 *   if (result == _old)
 *     *_ptr = _new;
 * unlock
 * return result == _old;
 *
 * Performance compare:
 * https://lkml.org/lkml/2010/12/18/59
 * https://software.intel.com/en-us/forums/intel-moderncode-for-parallel-architectures/topic/393587
 * Intel® 64 and IA-32 Architectures Optimization Reference Manual ( April 2012 ) on a Page 735
 */
