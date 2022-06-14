/* -----------------------------------------------------------------------------
 * Copyright (c) 2021 Arm Limited (or its affiliates). All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * -------------------------------------------------------------------------- */

#include "FreeRTOS.h"
#include "task.h"
#include <stdlib.h>
#include <string.h>

/* copied from heap_4.c */
typedef struct A_BLOCK_LINK
{
    struct A_BLOCK_LINK * pxNextFreeBlock; /*<< The next free block in the list. */
    size_t xBlockSize;                     /*<< The size of the free block. */
} BlockLink_t;

/* Override newlibc memory allocator functions */
void * malloc( size_t xLen )
{
    return pvPortMalloc( xLen );
}

void * calloc( size_t xNum,
               size_t xLen )
{
    void * pvBuffer = pvPortMalloc( xNum * xLen );

    if( pvBuffer != NULL )
    {
        ( void ) memset( pvBuffer, 0, xNum * xLen );
    }

    return pvBuffer;
}

#define SIZE_MASK    ( ~( 1U << 31 ) )
static const uintptr_t xHeapStructSize = ( sizeof( BlockLink_t ) + ( ( size_t ) ( portBYTE_ALIGNMENT - 1 ) ) ) & ~( ( size_t ) portBYTE_ALIGNMENT_MASK );

size_t malloc_usable_size( void * pvPtr )
{
    BlockLink_t * pxLink;
    uint8_t * puc;
    size_t xLen = 0;

    if( pvPtr != NULL )
    {
        configASSERT( ( uintptr_t ) pvPtr > xHeapStructSize );
        puc = ( uint8_t * ) pvPtr - xHeapStructSize;

        pxLink = ( void * ) puc;

        xLen = ( SIZE_MASK & pxLink->xBlockSize ) - xHeapStructSize;
        configASSERT( xLen >= 0 );
    }

    return xLen;
}

/* non-optimized realloc implementation */
void * realloc( void * pvPtr,
                size_t xNewLen )
{
    void * pvNewBuff = NULL;
    size_t xCurLen = 0;

    if( pvPtr == NULL )
    {
        pvNewBuff = pvPortMalloc( xNewLen );
    }
    else /* pvPtr is not NULL */
    {
        xCurLen = malloc_usable_size( pvPtr );

        /* New length is zero, free the block and return null */
        if( xNewLen == 0 )
        {
            vPortFree( pvPtr );
        }
        else
        {
            pvNewBuff = pvPortMalloc( xNewLen );
        }

        if( pvNewBuff != NULL )
        {
            if( xCurLen >= xNewLen )
            {
                ( void ) memcpy( pvNewBuff, pvPtr, xNewLen );
            }
            else /* xCurLen < xNewLen */
            {
                ( void ) memcpy( pvNewBuff, pvPtr, xCurLen );
            }

            /* Free the original buffer */
            vPortFree( pvPtr );
        }
    }

    /* Return the new buffer with copied data */
    return pvNewBuff;
}

void free( void * pvPtr )
{
    return vPortFree( pvPtr );
}

void explicit_bzero(void *s, size_t n) {
    memset( s, 0, n );
}
