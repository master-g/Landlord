/*
 *  mt19937.h
 *  kitsune
 *
 *  Created by Master.G on 13-8-19.
 *  Copyright (c) 2013 Master.G. All rights reserved.
 */

#ifndef MT19937_H_
#define MT19937_H_

#include "common.h"

#define MT_N    624

typedef struct mt19937_s
{
    uint32_t    mt[MT_N];   /* state vector */
    int32_t     mti;        /* mti == N+1 -> mt[N] not initialized */
    
} mt19937_t;

void Random_Init(mt19937_t *context, uint32_t seed);
void Random_InitWithArray(mt19937_t *context, uint32_t array[], int length);
uint32_t Random_uint32(mt19937_t *context);
int32_t Random_Int32(mt19937_t *context);
double Random_real_0_1(mt19937_t *context);

#endif /* MT19937_H_ */
