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

#include "lmath.h"

/* ************************************************************
 * MT19937 random number generator
 * ************************************************************/

#define M 397
#define MATRIX_A 0x9908B0DF   /* constant vector A */
#define UPPER_MASK 0x80000000 /* most significant w-r bits */
#define LOWER_MASK 0x7FFFFFFF /* least significant w-r bits */

#define FULL_MASK 0xFFFFFFFF
#define DEF_SEED 0x012BD6AA

void Random_Init(mt19937_t* context, uint32_t seed) {
  context->mt[0] = seed & FULL_MASK;

  for (context->mti = 1; context->mti < MT_N; context->mti++) {
    /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
    context->mt[context->mti] =
        (1812433253 * (context->mt[context->mti - 1] ^
                       (context->mt[context->mti - 1] >> 30)) +
         context->mti);
    context->mt[context->mti] &= FULL_MASK;
  }
}

void Random_InitWithArray(mt19937_t* context, uint32_t initarr[], int length) {
  int i, j, k;

  Random_Init(context, DEF_SEED);
  i = 1, j = 0;
  k = (MT_N > length ? MT_N : length);

  for (; k; k--) {
    context->mt[i] =
        (context->mt[i] ^
         ((context->mt[i - 1] ^ (context->mt[i - 1] >> 30)) * 1664525)) +
        initarr[j] + j;
    context->mt[i] &= FULL_MASK;
    i++;
    j++;

    if (j >= MT_N) {
      context->mt[0] = context->mt[MT_N - 1];
      i              = 1;
    }

    if (j >= length) {
      j = 0;
    }
  }

  for (k = MT_N - 1; k; k--) {
    context->mt[i] =
        (context->mt[i] ^
         ((context->mt[i - 1] ^ (context->mt[i - 1] >> 30)) * 1566083941)) -
        i;
    context->mt[i] &= FULL_MASK;
    i++;

    if (i >= MT_N) {
      context->mt[0] = context->mt[MT_N - 1];
      i              = 1;
    }
  }

  context->mt[0] = UPPER_MASK;
}

uint32_t Random_uint32(mt19937_t* context) {
  uint32_t y;
  int kk;
  static uint32_t mag01[2] = {0x0, MATRIX_A};

  /* mag01[x] = x * MATRIX_A for x = 0, 1 */

  if (context->mti >= MT_N) {
    if (context->mti == MT_N + 1)
      Random_Init(context, 5489);

    for (kk = 0; kk < MT_N - M; kk++) {
      y = (context->mt[kk] & UPPER_MASK) | (context->mt[kk + 1] & LOWER_MASK);
      context->mt[kk] = context->mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1];
    }

    for (; kk < MT_N - 1; kk++) {
      y = (context->mt[kk] & UPPER_MASK) | (context->mt[kk + 1] & LOWER_MASK);
      context->mt[kk] =
          context->mt[kk + (M - MT_N)] ^ (y >> 1) ^ mag01[y & 0x1];
    }
    y = (context->mt[MT_N - 1] & UPPER_MASK) | (context->mt[0] & LOWER_MASK);
    context->mt[MT_N - 1] = context->mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1];

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

int32_t Random_Int32(mt19937_t* context) {
  return (int32_t)(Random_uint32(context) >> 1);
}

double Random_real_0_1(mt19937_t* context) {
  return (double)(Random_uint32(context) * (1.0 / 4294967296.0));
}

/* ************************************************************
 * utils
 * ************************************************************/

/*
   https://compprog.wordpress.com/2007/10/17/generating-combinations-1/
   next_comb(int comb[], int k, int n)
   Generates the next combination of n elements as k after comb

   comb => the previous combination ( use (0, 1, 2, ..., k) for first)
   k => the size of the subsets to generate
   n => the size of the original set

   Returns: 1 if a valid combination was found
   0, otherwise
 */
int LMath_NextComb(int comb[], int k, int n) {
  int i = k - 1;

  ++comb[i];

  while ((i > 0) && (comb[i] >= n - k + 1 + i)) {
    --i;
    ++comb[i];
  }

  if (comb[0] > n - k) /* Combination (n-k, n-k+1, ..., n) reached */
    return 0;          /* No more combinations can be generated */

  /* comb now looks like (..., x, n, n, n, ..., n).
     Turn it into (..., x, x + 1, x + 2, ...) */
  for (i = i + 1; i < k; ++i)
    comb[i] = comb[i - 1] + 1;

  return 1;
}

void LMath_Shuffle(uint8_t* a, size_t n, mt19937_t* mt) {
  size_t i    = n, j;
  uint8_t tmp = 0;

  while (--i > 0) {
    if (mt != NULL)
      j = Random_Int32(mt) % (i + 1);
    else
      j = rand() % (i + 1);

    tmp  = a[j];
    a[j] = a[i];
    a[i] = tmp;
  }
}
