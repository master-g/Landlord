/*
 *  mt19937.c
 *  kitsune
 *
 *  Created by Master.G on 13-8-19.
 *  Copyright (c) 2013 Master.G. All rights reserved.
 */

#include "mt19937.h"

#define M           397
#define MATRIX_A    0x9908B0DF  /* constant vector A */
#define UPPER_MASK  0x80000000  /* most significant w-r bits */
#define LOWER_MASK  0x7FFFFFFF  /* least significant w-r bits */

#define FULL_MASK   0xFFFFFFFF
#define DEF_SEED    0x012BD6AA

void Random_Init(mt19937_t *context, uint32_t seed)
{
    context->mt[0] = seed & FULL_MASK;
    for (context->mti = 1; context->mti < MT_N; context->mti++)
    {
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        context->mt[context->mti] = (1812433253 * (context->mt[context->mti-1] ^ (context->mt[context->mti-1] >> 30)) + context->mti);
        context->mt[context->mti] &= FULL_MASK;
    }
}

void Random_InitWithArray(mt19937_t *context, uint32_t initarr[], int length)
{
    int i, j, k;
    Random_Init(context, DEF_SEED);
    i = 1, j = 0;
    k = (MT_N > length ? MT_N : length);
    for (; k; k--)
    {
        context->mt[i] = (context->mt[i] ^ ((context->mt[i-1] ^ (context->mt[i-1] >> 30)) * 1664525)) + initarr[j] + j;
        context->mt[i] &= FULL_MASK;
        i++;
        j++;
        if (j >= MT_N)
        {
            context->mt[0] = context->mt[MT_N-1];
            i = 1;
        }
        if (j >= length)
        {
            j = 0;
        }
    }
    
    for (k = MT_N-1; k; k--)
    {
        context->mt[i] = (context->mt[i] ^ ((context->mt[i-1] ^ (context->mt[i-1] >>30)) * 1566083941)) - i;
        context->mt[i] &= FULL_MASK;
        i++;
        if (i >= MT_N)
        {
            context->mt[0] = context->mt[MT_N-1];
            i = 1;
        }
    }
    
    context->mt[0] = UPPER_MASK;
}

uint32_t Random_uint32(mt19937_t *context)
{
    uint32_t y;
    int kk;
    static uint32_t mag01[2] = { 0x0, MATRIX_A };
    /* mag01[x] = x * MATRIX_A for x = 0, 1 */
    
    if (context->mti >= MT_N)
    {
        if (context->mti == MT_N+1)
            Random_Init(context, 5489);
        
        for (kk = 0; kk < MT_N-M; kk++)
        {
            y = (context->mt[kk] & UPPER_MASK) | (context->mt[kk + 1] & LOWER_MASK);
            context->mt[kk] = context->mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        
        for (; kk < MT_N-1; kk++)
        {
            y = (context->mt[kk] & UPPER_MASK) | (context->mt[kk + 1] & LOWER_MASK);
            context->mt[kk] = context->mt[kk + (M-MT_N)] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        y = (context->mt[MT_N-1] & UPPER_MASK) | (context->mt[0] & LOWER_MASK);
        context->mt[MT_N-1] = context->mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1];
        
        context->mti = 0;
    }
    
    y = context->mt[context->mti++];
    
    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9D2C5680;
    y ^= (y << 15) & 0xEFC60000;
    y ^= (y >> 18);
    
    return y;
}

int32_t Random_int32(mt19937_t *context)
{
    return (int32_t)(Random_uint32(context)>>1);
}

double Random_real_0_1(mt19937_t *context)
{
    return (double)(Random_uint32(context)*(1.0/4294967296.0));
}
