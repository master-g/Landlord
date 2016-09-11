/*
The MIT License (MIT)

Copyright (c) 2016 Master.G

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

#ifndef LANDLORD_CARD_H_
#define LANDLORD_CARD_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * here is how a card is represent in a 32 bit unsigned integer
 * ------------------------------------------------------------------
 * 32            24 23            16               8  7             0
 * ------------------------------------------------------------------
 * x x b b b b b b  b b b b b b b b  c d h s r r r r  x x p p p p p p
 *
 * x: reserved bits
 * b: binary represent of the card's rank
 * c: club
 * d: diamond
 * h: heart
 * s: spade
 * r: hex represent of the card's rank
 * p: prime number represent of the card's rank
 *
 * ---
 *
 * prime number represent of the card's rank
 *
 * prime
 * 2 3 5 7 11 13 17 19 23 29 31 37 41 43    47
 * rank
 * 3 4 5 6 7  8  9  10 J  Q  K  A  2  joker JOKER
 *
 * ---
 *
 * thus we have
 */

#define CARD_BIT_OFFSET_PRIME   0
#define CARD_BIT_OFFSET_RANK    8
#define CARD_BIT_OFFSET_SUIT    12
#define CARD_BIT_OFFSET_BIN     16

#define CARD_PRIME_3  0x02
#define CARD_PRIME_4  0x03
#define CARD_PRIME_5  0x05
#define CARD_PRIME_6  0x07
#define CARD_PRIME_7  0x0B
#define CARD_PRIME_8  0x0D
#define CARD_PRIME_9  0x11
#define CARD_PRIME_T  0x13
#define CARD_PRIME_J  0x17
#define CARD_PRIME_Q  0x1D
#define CARD_PRIME_K  0x1F
#define CARD_PRIME_A  0x25
#define CARD_PRIME_2  0x29
#define CARD_PRIME_r  0x2B
#define CARD_PRIME_R  0x2F

#define CARD_RANK_3   0x01
#define CARD_RANK_4   0x02
#define CARD_RANK_5   0x03
#define CARD_RANK_6   0x04
#define CARD_RANK_7   0x05
#define CARD_RANK_8   0x06
#define CARD_RANK_9   0x07
#define CARD_RANK_T   0x08
#define CARD_RANK_J   0x09
#define CARD_RANK_Q   0x0A
#define CARD_RANK_K   0x0B
#define CARD_RANK_A   0x0C
#define CARD_RANK_2   0x0D
#define CARD_RANK_r   0x0E
#define CARD_RANK_R   0x0F

#define CARD_SUIT_SPADE     0x01
#define CARD_SUIT_HEART     0x02
#define CARD_SUIT_DIAMOND   0x04
#define CARD_SUIT_CLUB      0x08

#define CARD_BIN_3    0x0001
#define CARD_BIN_4    0x0002
#define CARD_BIN_5    0x0004
#define CARD_BIN_6    0x0008
#define CARD_BIN_7    0x0010
#define CARD_BIN_8    0x0020
#define CARD_BIN_9    0x0040
#define CARD_BIN_T    0x0080
#define CARD_BIN_J    0x0100
#define CARD_BIN_Q    0x0200
#define CARD_BIN_K    0x0400
#define CARD_BIN_A    0x0800
#define CARD_BIN_2    0x1000
#define CARD_BIN_r    0x2000
#define CARD_BIN_R    0x4000

#define CARD_SET_LENGTH     54

#define CARD_RANK(x)        (uint32_t)(((x) >> 8) & 0x0F)
#define CARD_SUIT(x)        (uint32_t)(((x) >> 8) & 0x0F0)

uint32_t Card_Make(uint32_t suit, uint32_t rank);

#define CARD_RANK_BEG   CARD_RANK_3
#define CARD_RANK_END   (CARD_RANK_R + 1)

/*
 * ************************************************************
 * card array
 * ************************************************************
 */

#define CARD_ARRAY_PRESET_LENGTH    CARD_SET_LENGTH

#define CardArray_GetFront(a)   ((a)->cards[0])
#define CardArray_GetBack(a)    ((a)->cards[(a)->lenth])
#define CardArray_Peek(a, i)    ((a)->cards[(i)])
#define CardArray_Clear(a)      (memset((a), 0, sizeof(card_array_t)))
#define CardArray_Copy(d, s)     (memcpy((d), (s), sizeof(card_array_t)))
#define CardArray_IsFull(a)     ((a)->length >= CARD_SET_LENGTH)
#define CardArray_IsEmpty(a)    ((a)->length == 0)
#define CardArray_Capacity(a)   (CARD_ARRAY_PRESET_LENGTH - (a)->length)
#define CardArray_Set(array, where, what, count)    (memset((array->cards + where), (what), (count) * sizeof(uint32_t)))

typedef struct _card_arr_s {
    int length;
    uint32_t cards[CARD_ARRAY_PRESET_LENGTH];

} card_array_t;

/* sort function */
typedef int (*CardSortFunc)(void *, void *);

/**
 * Initialize a card array from string
 * 
 * @param  array The result card array
 * @param  str   The source string
 * @return       For method chaining
 */
void *CardArray_InitFromString(card_array_t *array, const char *str);

/**
 * Reset a card array with 54 cards
 * 
 * @param array The card array to reset with
 */
void CardArray_Reset(card_array_t *array);

/*
 * concatenates two card arrays
 */
int CardArray_Concat(card_array_t *head, card_array_t *tail);

/*
 * subtract two card arrays
 */
void CardArray_Subtract(card_array_t *from, card_array_t *sub);

/*
 * check for identity
 */
int CardArray_IsIdentity(card_array_t *a, card_array_t *b);

/*
 * check for contain
 */
int CardArray_IsContain(card_array_t *array, card_array_t *segment);

/*
 * push a card to the rear of the array
 */
void CardArray_PushBack(card_array_t *array, uint32_t card);

/*
 * push a card to the front of the array
 */
uint32_t CardArray_PushFront(card_array_t *array, uint32_t card);

/*
 * pop a card from the front of the array
 */
uint32_t CardArray_PopFront(card_array_t *array);

/* 
 * pop a card from the back of the array
 */
uint32_t CardArray_PopBack(card_array_t *array);

/*
 * drop multiple cards from the front of the array
 */
int CardArray_DropFront(card_array_t *array, int count);

/*
 * drop multiple cards from the back of the array
 */
int CardArray_DropBack(card_array_t *array, int count);

/*
 * insert a card to the front of index
 */
void CardArray_Insert(card_array_t *array, int before, uint32_t card);

/*
 * remove a card from the front of index
 */
uint32_t CardArray_Remove(card_array_t *array, int where);

/*
 * remove a card from array
 */
uint32_t CardArray_RemoveCard(card_array_t *array, uint32_t card);

/*
 * push back multiple cards from array
 */
int CardArray_PushBackCards
        (card_array_t *array, card_array_t *from, int where, int count);

/*
 * transfer specific rank cards from array to array
 */
void CardArray_CopyRank(card_array_t *dst, card_array_t *src, uint32_t rank);

/*
 * remove specific rank cards from array
 */
void CardArray_RemoveRank(card_array_t *array, uint32_t rank);

/*
 * sort cards
 */
void CardArray_Sort
        (card_array_t *array, int (*comparator)(const void *, const void *));

/*
 * reverse cards
 */
void CardArray_Reverse(card_array_t *array);

/*
 * print every card in the array
 */
void CardArray_Print(card_array_t *array);

/*
 * convert a card to string
 */
int Card_ToString(uint32_t card, char *buf, int len);

/*
 * ************************************************************
 * helper function
 * ************************************************************
 */

int rk_next_comb(int comb[], int k, int n);

#ifdef __cplusplus
}
#endif

#endif /* LANDLORD_CARD_H_ */
