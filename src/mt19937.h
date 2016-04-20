/*
The MIT License (MIT)

Copyright (c) 2014 Master.G

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in 
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef MT19937_H_
#define MT19937_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MT_N    624

typedef struct mt19937_s {
  uint32_t mt[MT_N];
  /* state vector */
  int32_t mti;        /* mti == N+1 -> mt[N] not initialized */

} mt19937_t;

void Random_Init(mt19937_t *context, uint32_t seed);
void Random_InitWithArray(mt19937_t *context, uint32_t array[], int length);
uint32_t Random_uint32(mt19937_t *context);
int32_t Random_Int32(mt19937_t *context);
double Random_real_0_1(mt19937_t *context);

#ifdef __cplusplus
}
#endif

#endif /* MT19937_H_ */
