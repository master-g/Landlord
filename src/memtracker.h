/**
 *  @file    memtracker.h
 *  @brief   memory tracker
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2013/02/11
 *  Company:  SNSTEAM.inc
 *  (C) Copyright 2013 SNSTEAM.inc All rights reserved.
 * 
 * This file is a part of kitsune
 *
 * The copyright to the contents herein is the property of SNSTEAM.inc
 * The contents may be used and/or copied only with the written permission of
 * SNSTEAM.inc or in accordance with the terms and conditions stipulated in
 * the agreement/contract under which the contents have been supplied.
 * =====================================================================================
 */

#ifndef KITSUNE_MEMTRACKER_H_
#define KITSUNE_MEMTRACKER_H_

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* #define KITSUNE_DEBUG */

#ifdef KITSUNE_DEBUG
    #ifndef INTERNAL
        #define malloc(size)        memtrack_malloc(size, #size, __FILE__, __LINE__)
        #define calloc(count, size) memtrack_calloc(count, size, #size, __FILE__, __LINE__)
        #define realloc(ptr, size)  memtrack_realloc(ptr, #ptr, size, #size, __FILE__, __LINE__)
        #define free(ptr)           memtrack_free(ptr, #ptr, __FILE__, __LINE__)
    #endif /* INTERNAL */

#endif /* KITSUNE_DEBUG */
    
void* memtrack_malloc(size_t size, const char* expr, const char* file, int line);
void* memtrack_calloc(size_t count, size_t elem_size, const char* expr, const char* file, int line);
void* memtrack_realloc(void* ptr, const char* eptr, size_t size, const char* expr, const char* file, int line);
void memtrack_free(void* ptr, const char* expr, const char* file, int line);
void memtrack_list_allocations(void);

#ifdef __cplusplus
}
#endif

#endif /* KITSUNE_MEMTRACKER_H_ */
