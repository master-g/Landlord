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
#include "ruiko_algorithm.h"

#ifdef __cplusplus
extern "C" {
#endif

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
#define HAND_CHAIN_NONE         0x00
#define HAND_CHAIN              0x80

#define HAND_NONE               0x00
#define HAND_SEARCHER_MASK      0xFF

#define Hand_GetPrimal(h)       ((h)&0x0F)
#define Hand_GetKicker(h)       ((h)&0x70)
#define Hand_GetChain(h)        ((h)&0x80)

#define Hand_SetPrimal(h, p)     ((h)|=(p))
#define Hand_SetKicker(h, k)     Hand_SetPrimal(h,k)
#define Hand_SetChain(h, c)      Hand_SetPrimal(h,c)

#define Hand_Format(p, k, c)      (uint8_t)((p)|(k)|(c))
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
 * count ranks in a card array
 */
void Hand_CountRank(card_array_t *array, int *count, int *sort);

/*
 * parse a card array to hand
 */
void Hand_Parse(hand_t *hand, card_array_t *array);

/*
 * compare two hands
 */
int Hand_Compare(hand_t *a, hand_t *b);

/*
 * hand print
 */
void Hand_Print(hand_t *hand);

/*
 * ************************************************************
 * hand list
 * ************************************************************
 */

typedef int (*HandList_EvaluateFunc)(card_array_t *);

/*
 * push a hand into the front of hand list
 */
void HandList_PushFront(rk_list_t **hl, hand_t *hand);

/*
 * remove a hand from hand list
 */
void HandList_Remove(rk_list_t **hl, rk_list_t *node);

/*
 * search a specific hand type from hand list
 */
rk_list_t *HandList_Find(rk_list_t **hl, int handtype);

/*
 * destroy a hand list
 */
void HandList_Destroy(rk_list_t **hl);

/*
 * get payload as hand_t
 */
hand_t *HandList_GetHand(rk_list_t *node);

/*
 * ************************************************************
 * utils
 * ************************************************************
 */

/*
 * search a beat in card array
 */
int HandList_SearchBeat(card_array_t *cards, hand_t *tobeat, hand_t *beat);

/*
 * search all the beats
 */
rk_list_t *HandList_SearchBeatList(card_array_t *cards, hand_t *tobeat);

/*
 * standard analyze a card array into hand list
 */
rk_list_t *HandList_StandardAnalyze(card_array_t *array);

/*
 * count how many primal hands in array
 */
int HandList_StandardEvaluator(card_array_t *array);

/*
 * advanced hand analyze
 */
rk_list_t *HandList_AdvancedAnalyze(card_array_t *array);

/*
 * advanced hand evaluator
 */
int HandList_AdvancedEvaluator(card_array_t *array);

/*
 * search best beat from given cards
 */
int HandList_BestBeat(card_array_t *array, hand_t *tobeat, hand_t *beat, HandList_EvaluateFunc func);

/*
 * print hand_list_t
 */
void HandList_Print(rk_list_t *hl);

#ifdef __cplusplus
}
#endif

#endif /* LANDLORD_HAND_H_ */
