/****************************************************************************
 * mm/mm_zalloc.c
 *
 *   Copyright (C) 2007, 2009, 2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdlib.h>
#include <string.h>

#include <nuttx/mm.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#if defined(CONFIG_ARCH_ADDRENV) && defined(CONFIG_BUILD_KERNEL)
/* In the kernel build, there a multiple user heaps; one for each task
 * group.  In this build configuration, the user heap structure lies
 * in a reserved region at the beginning of the .bss/.data address
 * space (CONFIG_ARCH_DATA_VBASE).  The size of that region is given by
 * ARCH_DATA_RESERVE_SIZE
 */

#  include <nuttx/addrenv.h>
#  define USR_HEAP (&ARCH_DATA_RESERVE->ar_usrheap)

#else
/* Otherwise, the user heap data structures are in common .bss */

#  define USR_HEAP &g_mmheap;
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/************************************************************************
 * Name: zalloc
 *
 * Description:
 *   Allocate and zero memory from the user heap.
 *
 * Parameters:
 *   size - Size (in bytes) of the memory region to be allocated.
 *
 * Return Value:
 *   The address of the allocated memory (NULL on failure to allocate)
 *
 ************************************************************************/

FAR void *zalloc(size_t size)
{
#ifdef CONFIG_ARCH_ADDRENV
  /* Use malloc() because it implements the sbrk() logic */

  FAR void *alloc = malloc(size);
  if (alloc)
    {
       memset(alloc, 0, size);
    }

  return alloc;

#else
  /* Use mm_zalloc() becuase it implements the clear */

  return mm_zalloc(USR_HEAP, size);
#endif
}
