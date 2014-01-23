/*
 *  hand.c
 *  Landlord
 *
 *  Created by Master.G on 14-1-20.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

/*
 * http://scim.brad.ac.uk/staff/pdf/picowlin/AISB2011.pdf
 * http://en.wikipedia.org/wiki/Monte-Carlo_tree_search
 */

#include "hand.h"

#define HAND_MIN_LENGTH         1
#define HAND_MAX_LENGTH         20

/*
 * ************************************************************
 * pattern
 * ************************************************************
 */

#define PATTERN_LENGTH 12

#define HANDS_PATTERN_NONE  0
#define HANDS_PATTERN_1     1
#define HANDS_PATTERN_2     2
#define HANDS_PATTERN_3     3
#define HANDS_PATTERN_4_1   4
#define HANDS_PATTERN_4_2   5
#define HANDS_PATTERN_5_1   6
#define HANDS_PATTERN_5_2   7
#define HANDS_PATTERN_6_1   8
#define HANDS_PATTERN_6_2   9
#define HANDS_PATTERN_6_3   10
#define HANDS_PATTERN_6_4   11
#define HANDS_PATTERN_7_1   12
#define HANDS_PATTERN_8_1   13
#define HANDS_PATTERN_8_2   14
#define HANDS_PATTERN_8_3   15
#define HANDS_PATTERN_8_4   16
#define HANDS_PATTERN_9_1   17
#define HANDS_PATTERN_9_2   18
#define HANDS_PATTERN_10_1  19
#define HANDS_PATTERN_10_2  20
#define HANDS_PATTERN_10_3  21
#define HANDS_PATTERN_11    22
#define HANDS_PATTERN_12_1  23
#define HANDS_PATTERN_12_2  24
#define HANDS_PATTERN_12_3  25
#define HANDS_PATTERN_12_4  26
#define HANDS_PATTERN_14    27
#define HANDS_PATTERN_16_1  28
#define HANDS_PATTERN_16_2  29
#define HANDS_PATTERN_18    30
#define HANDS_PATTERN_20_1  31
#define HANDS_PATTERN_20_2  32
#define HANDS_PATTERN_END   33


int _hand_pattern[][PATTERN_LENGTH] =
{
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     /* place holder */
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     /* 1, solo */
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     /* 2, pair / nuke */
    { 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     /* 3, trio */
    { 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     /* 4, bomb */
    { 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     /* 4, trio solo */
    { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },     /* 5, solo chain */
    { 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     /* 5, trio pair */
    { 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },     /* 6, solo chain */
    { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     /* 6, pair chain */
    { 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     /* 6, trio chain */
    { 4, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     /* 6, four dual solo */
    { 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },     /* 7, solo chain */
    { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },     /* 8, solo chain */
    { 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0 },     /* 8, pair chain */
    { 3, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },     /* 8, trio solo chain */
    { 4, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     /* 8, four dual pair */
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 },     /* 9, solo chain */
    { 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     /* 9, trio chain */
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },     /* 10, solo chain */
    { 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0 },     /* 10, pair chain */
    { 3, 3, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0 },     /* 10, trio pair chain */
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },     /* 11, solo chain */
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },     /* 12, solo chain */
    { 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0 },     /* 12, pair chain */
    { 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0 },     /* 12, trio chain */
    { 3, 3, 3, 1, 1, 1, 0, 0, 0, 0, 0, 0 },     /* 12, trio solo chain */
    { 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0 },     /* 14, pair chain */
    { 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0 },     /* 16, pair chain */
    { 3, 3, 3, 3, 1, 1, 1, 1, 0, 0, 0, 0 },     /* 16, trio solo chain */
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0 },     /* 18, pair chain */
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0 },     /* 20, pair chain */
    { 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 0, 0 }      /* 20, trio solo chain */
};

/*
 * ************************************************************
 * hand
 * ************************************************************
 */

hand_t *Hand_Create(void)
{
    hand_t *hand = (hand_t *)calloc(1, sizeof(hand_t));
    hand->cards = CardArray_CreateEmpty();
    
    return hand;
}


void Hand_Destroy(hand_t *hand)
{
    CardArray_Destroy(hand->cards);
    free(hand);
}

void Hand_Clear(hand_t *hand)
{
    CardArray_Clear(hand->cards);
    hand->type = 0;
}

int Hand_Validate(hand_t *hand)
{
    return 0;
}

/*
 * ************************************************************
 * parser
 * ************************************************************
 */

/* sort function for rank count array */
int _Hand_PatternSort(const void *a, const void *b)
{
    return *(int *)b - *(int *)a;
}

/* count rank in card array */
void _Hand_CountRank(card_array_t *array, int *count)
{
    int i = 0;
    memset(count, 0, sizeof(int) * CARD_RANK_END);
    for (i = 0; i < array->length; i++)
        count[CARD_RANK(array->cards[i])]++;
    
    qsort(count, PATTERN_LENGTH, sizeof(int), _Hand_PatternSort);
}

int _Hand_PatternMatch(int *count, int pattern)
{
    int i = 0;
    int ret = 1;
    
    for (i = 0; i < PATTERN_LENGTH; i++)
    {
        if (count[i] != _hand_pattern[pattern][i])
        {
            ret = 0;
            break;
        }
    }
    
    return ret;
}

int _Hand_CheckChain(int *count, int num, int expectLength)
{
    int i = 0;
    int marker = 0;
    int length = 0;
    
    for (i = CARD_RANK_BEG; i < CARD_RANK_END; i++)
    {
        /* joker and 2 are forbidden for solo-chain */
        if (i == CARD_RANK_R || i == CARD_RANK_2)
            break;
        
        if (count[i] != 0 && count[i] != num)
            break;
        
        if (count[i] == num && marker == 0)
            marker = i;
        
        if (count[i] == 0 && marker != 0)
        {
            length = i - marker + 1;
            break;
        }
    }
    return (length == expectLength ? 1 : 0);
}

void _Hand_Parse_1(hand_t *hand, card_array_t *array)
{
    /* solo */
    CardArray_Copy(hand->cards, array);
    hand->type = HAND_PRIMAL_SOLO | HAND_KICKER_NONE | HAND_CHAIN_NONE;
}

void _Hand_Parse_2(hand_t *hand, card_array_t *array)
{
    /* pair or nuke */
    if (array->cards[0] == array->cards[1])
    {
        hand->type = CARD_RANK(array->cards[0]) == CARD_RANK_R ? HAND_PRIMAL_NUKE : HAND_PRIMAL_PAIR;
        CardArray_Copy(hand->cards, array);
    }
}

void _Hand_Parse_3(hand_t *hand, card_array_t *array)
{
    /* trio */
    if (CARD_RANK(array->cards[0]) ==
        CARD_RANK(array->cards[1]) ==
        CARD_RANK(array->cards[2]))
    {
        CardArray_Copy(hand->cards, array);
        hand->type = HAND_PRIMAL_TRIO;
    }
}

void _Hand_Parse_4(hand_t *hand, card_array_t *array)
{
    int count[CARD_RANK_END];
    int i = 0;
    int trio = 0;
    uint8_t kicker = 0;
    
    /* trio solo | bomb */
    if (CARD_RANK(array->cards[0]) ==
        CARD_RANK(array->cards[1]) ==
        CARD_RANK(array->cards[2]) ==
        CARD_RANK(array->cards[3]))
    {
        CardArray_Copy(hand->cards, array);
        hand->type = HAND_PRIMAL_BOMB;
    }
    else
    {
        _Hand_CountRank(array, count);
        for (i = CARD_RANK_BEG; i < CARD_RANK_END; i++)
        {
            if (count[i] == 3)
            {
                /* trio found */
                trio = i;
                break;
            }
        }
        
        if (trio != 0)
        {
            /* format trio into AAAB style */
            for (i = 0; i < array->length; i++)
            {
                if (CARD_RANK(array->cards[i]) == trio)
                    CardArray_PushBack(hand->cards, array->cards[i]);
                else
                    kicker = array->cards[i];
            }
            
            CardArray_PushBack(hand->cards, kicker);
            hand->type = HAND_PRIMAL_TRIO | HAND_KICKER_SOLO | HAND_CHAIN_NONE;
        }
    }
}

void _Hand_Parse_5(hand_t *hand, card_array_t *array)
{
    int count[CARD_RANK_END];
    int i = 0;
    int j = 0;
    int trio = 0;
    int pair = 0;
    card_array_t pairArray;
    
    CardArray_Clear(&pairArray);
    
    _Hand_CountRank(array, count);
    
    for (i = CARD_RANK_BEG; i < CARD_RANK_END; i++)
    {
        /* trio found , possible trio pair */
        if (count[i] == 3)
            trio = i;
        
        /* 
         * R = joker, count = 2 means nuke 
         * and nuke can not be trio-pair's kicker
         */
        if (count[i] == 2 && i != CARD_RANK_R)
            pair = 2;
        
        if (trio != 0 && pair != 0)
        {
            /* 
             * trio pair found 
             * format trio pair into AAABB style
             */
            for (j = 0; j < array->length; j++)
            {
                if (CARD_RANK(array->cards[j]) == trio)
                    CardArray_PushBack(hand->cards, array->cards[i]);
                else
                    CardArray_PushBack(&pairArray, array->cards[i]);
            }
            
            CardArray_Concate(hand->cards, &pairArray);
            hand->type = HAND_PRIMAL_TRIO | HAND_KICKER_PAIR | HAND_CHAIN_NONE;
            
            break;
        }
    }
    
    /*
     * trio pair not found, solo-chain left
     * as solo-chain, trio and pair must be 0
     */
    if (hand->type == 0 && trio == 0 && pair == 0)
    {
        if (_Hand_CheckChain(count, 1, 5))
        {
            CardArray_Copy(hand->cards, array);
            hand->type = HAND_PRIMAL_SOLO | HAND_KICKER_NONE | HAND_CHAIN;
        }
    }
}

void _Hand_Parse_6(hand_t *hand, card_array_t *array)
{
    int count[CARD_RANK_END];
    
    _Hand_CountRank(array, count);
}

void _Hand_Parse_Default(hand_t *hand, card_array_t *array)
{
    hand->type = 0;
}

typedef void (* Hand_Parser)(hand_t *, card_array_t *);

void Hand_Parse(hand_t *hand, card_array_t *array)
{
    Hand_Parser parser[HAND_MAX_LENGTH + 1];
    
    CardArray_Sort(array, NULL);
    
    if (array->length < HAND_MIN_LENGTH || array->length > HAND_MAX_LENGTH)
    {
        hand->type = HAND_PRIMAL_NONE;
    }
    else
    {
        parser[array->length](hand, array);
    }
}

/*
 * ************************************************************
 * comparators
 * ************************************************************
 */

/* one of a, b must be bomb or nuke */
int _Hand_CompareBomb(hand_t *a, hand_t *b)
{
    int ret = HAND_CMP_ILLEGAL;
    
    /* same type same cards, equal */
    if ((a->type == b->type) && (a->cards->cards[0] == b->cards->cards[0]))
        ret = HAND_CMP_EQUAL;
    /* both are bombs, compare by card rank */
    else if (a->type == HAND_PRIMAL_BOMB && b->type == HAND_PRIMAL_BOMB)
        ret = CARD_RANK(a->cards->cards[0]) > CARD_RANK(b->cards->cards[0]) ? HAND_CMP_GREATER : HAND_CMP_LESS;
    else
        ret = Hand_GetPrimal(a->type) > Hand_GetPrimal(b->type) ? HAND_CMP_GREATER : HAND_CMP_LESS;
    
    return ret;
}

int Hand_Compare(hand_t *a, hand_t *b)
{
    int result = HAND_CMP_ILLEGAL;
    
    /* 
     * different hand type
     * check for bomb and nuke
     */
    if (a->type != b->type)
    {
        if (a->type != HAND_PRIMAL_NUKE &&
            a->type != HAND_PRIMAL_BOMB &&
            b->type != HAND_PRIMAL_NUKE &&
            b->type != HAND_PRIMAL_BOMB)
        {
            result = HAND_CMP_ILLEGAL;
        }
        else
        {
            result = _Hand_CompareBomb(a, b);
        }
    }
    else /* same hand type and with no bombs */
    {
        /* same hand type but different length */
        if (a->cards->length != b->cards->length)
        {
            result = HAND_CMP_ILLEGAL;
        }
        else /* same hand type and same length */
        {
            if (CARD_RANK(a->cards->cards[0]) == CARD_RANK(b->cards->cards[0]))
                result = HAND_CMP_EQUAL;
            else
                result = CARD_RANK(a->cards->cards[0]) > CARD_RANK(b->cards->cards[0]) ? HAND_CMP_GREATER : HAND_CMP_LESS;
        }
    }
    
    return result;
}

