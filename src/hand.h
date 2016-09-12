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

#ifndef LANDLORD_HAND_H_
#define LANDLORD_HAND_H_

#include "card.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HAND_MIN_LENGTH             1
#define HAND_MAX_LENGTH             20
#define HAND_SOLO_CHAIN_MIN_LENGTH  5
#define HAND_PAIR_CHAIN_MIN_LENGTH  6
#define HAND_TRIO_CHAIN_MIN_LENGTH  6
#define HAND_FOUR_CHAIN_MIN_LENGTH  8

/* primal hands */
#define HAND_PRIMAL_NONE        0x00
#define HAND_PRIMAL_NUKE        0x06
#define HAND_PRIMAL_BOMB        0x05
#define HAND_PRIMAL_FOUR        0x04
#define HAND_PRIMAL_TRIO        0x03
#define HAND_PRIMAL_PAIR        0x02
#define HAND_PRIMAL_SOLO        0x01

/* kicker hands */
#define HAND_KICKER_NONE        0x00
#define HAND_KICKER_SOLO        0x10
#define HAND_KICKER_PAIR        0x20
#define HAND_KICKER_DUAL_SOLO   0x30
#define HAND_KICKER_DUAL_PAIR   0x40

/* chain */
#define HAND_CHAINLESS          0x00
#define HAND_CHAIN              0x80

#define HAND_NONE               0x00
#define HAND_SEARCHER_MASK      0xFF

#define Hand_GetPrimal(h)       ((h)&0x0F)
#define Hand_GetKicker(h)       ((h)&0x70)
#define Hand_GetChain(h)        ((h)&0x80)

#define Hand_SetPrimal(h, p)    ((h)|=(p))
#define Hand_SetKicker(h, k)    Hand_SetPrimal(h,k)
#define Hand_SetChain(h, c)     Hand_SetPrimal(h,c)

#define Hand_Format(p, k, c)    (uint8_t)((p)|(k)|(c))
#define Hand_ClearPrimal(h)     ((h)&0xF0)
#define Hand_ClearKicker(h)     ((h)&0x8F)
#define Hand_ClearChain(h)      ((h)&0x7F)

typedef enum {
  HAND_CMP_ILLEGAL = -3,
  HAND_CMP_LESS = -1,
  HAND_CMP_EQUAL = 0,
  HAND_CMP_GREATER = 1

} HandCompareResult;

/*
 * hand is a valid card set that can play.
 * cards format must be like 12345/112233/111222/1112223344/11122234 etc
 */
typedef struct _hand_s {
  uint8_t type;
  card_array_t cards;

} hand_t;

/*
 * clear a hand
 */
void Hand_Clear(hand_t *hand);

/*
 * copy hands
 */
void Hand_Copy(hand_t *dst, hand_t *src);

/*
 * parse a card array to hand
 */
int Hand_Parse(hand_t *hand, card_array_t *array);

/*
 * compare two hands
 */
int Hand_Compare(hand_t *a, hand_t *b);

/*
 * hand print
 */
void Hand_Print(hand_t *hand);

#ifdef __cplusplus
}
#endif

#endif /* LANDLORD_HAND_H_ */
