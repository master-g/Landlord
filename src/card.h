/*
 *  Header.h
 *  Landlord
 *
 *  Created by Master.G on 14-1-17.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#ifndef LANDLORD_CARD_H_
#define LANDLORD_CARD_H_

#include "common.h"

#define CARD_RANK_3     0x01
#define CARD_RANK_4     0x02
#define CARD_RANK_5     0x03
#define CARD_RANK_6     0x04
#define CARD_RANK_7     0x05
#define CARD_RANK_8     0x06
#define CARD_RANK_9     0x07
#define CARD_RANK_T     0x08
#define CARD_RANK_J     0x09
#define CARD_RANK_Q     0x0A
#define CARD_RANK_K     0x0B
#define CARD_RANK_A     0x0C
#define CARD_RANK_2     0x0D
#define CARD_RANK_R     0x0E

#define CARD_RANK_BEG   0x01
#define CARD_RANK_END   0x0F

#define CARD_SUIT_CLUB      0x10
#define CARD_SUIT_DIAMOND   0x20
#define CARD_SUIT_HEART     0x30
#define CARD_SUIT_SPADE     0x40

#define CARD_SET_LENGTH     54

#define	CARD_RANK(x)		((x) & 0x0F)
#define CARD_SUIT(x)        ((x) & 0xF0)

#define CARD_RED(x)         (CARD_SUIT(x) == CARD_SUIT_CLUB || CARD_SUIT(x) == CARD_SUIT_HEART)

#define Card_Make(suit, rank) ((suit)|(rank))

/*
 * ************************************************************
 * card array
 * ************************************************************
 */

#define CARD_ARRAY_PRESET_LENGTH    54

#define CardArray_GetFront(a)   ((a)->cards[0])
#define CardArray_GetBack(a)    ((a)->cards[(a)->lenth])
#define CardArray_Peek(a, i)    ((a)->cards[(i)])
#define CardArray_Clear(a)      (memset((a), 0, sizeof(card_array_t)))
#define CardArray_Copy(d,s)     (memcpy((d), (s), sizeof(card_array_t)))
#define CardArray_IsFull(a)     ((a)->length >= CARD_SET_LENGTH)
#define CardArray_IsEmpty(a)    ((a)->length == 0)
#define CardArray_Capacity(a)   (CARD_ARRAY_PRESET_LENGTH - (a)->length)

typedef struct card_array_t
{
    int     length;
    uint8_t cards[CARD_ARRAY_PRESET_LENGTH];
    
} card_array_t;

/* sort function */
typedef int (*CardSortFunc)(void *, void *);

/*
 * creat a empty card array
 */
card_array_t *CardArray_CreateEmpty(void);

/*
 * create a card array with 54 poker cards
 */
card_array_t *CardArray_CreateSet(void);

/*
 * parse a string to card array
 */
card_array_t *CardArray_CreateFromString(const char *str);

/*
 * destroy a card array
 */
void CardArray_Destroy(card_array_t *array);

/*
 * reset a card array with 54 cards
 */
void CardArray_Reset(card_array_t *array);

/*
 * concate two card arrays
 */
int CardArray_Concate(card_array_t *head, card_array_t *tail);

/*
 * subtract two card arrays
 */
void CardArray_Subtract(card_array_t *from, card_array_t *sub);

/*
 * push a card to the rear of the array
 */
uint8_t CardArray_PushBack(card_array_t *array, uint8_t card);

/*
 * push a card to the front of the array
 */
uint8_t CardArray_PushFront(card_array_t *array, uint8_t card);

/*
 * pop a card from the front of the array
 */
uint8_t CardArray_PopFront(card_array_t *array);

/* 
 * pop a card from the back of the array
 */
uint8_t CardArray_PopBack(card_array_t *array);

/*
 * insert a card to the front of index
 */
uint8_t CardArray_Insert(card_array_t *array, int before, uint8_t card);

/*
 * remove a card from the front of index
 */
uint8_t CardArray_Remove(card_array_t *array, int where);

/*
 * transfer specific rank cards from array to array
 */
void CardArray_TransferRank(card_array_t *dst, card_array_t *src, uint8_t rank);

/*
 * sort cards
 */
void CardArray_Sort(card_array_t *array, int (*comparator)(const void *, const void *));

/*
 * print every card in the array
 */
void CardArray_Print(card_array_t *array);

/*
 * convert a card to string
 */
int Card_ToString(uint8_t card, char *buf, int len);

/*
 * test
 */
void CardArray_Test(void);


#endif /* LANDLORD_CARD_H_ */