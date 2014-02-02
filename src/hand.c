/*
 *  hand.c
 *  Landlord
 *
 *  Created by Master.G on 14-1-20.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

/* 
 * brief about Hand_Parse function
 * --------------------------------------
 *
 * Landlord has few types of hands,
 * for example, solo-chain as 34567, trio-solo-chain as 333444555A26
 * their patterns are 11111, 333111
 * which can help us to determine the type of a specific hand
 *
 * for exampe, a 6 cards can have 4 types of hands
 * solo-chain, pair-chain, trio-chain and four-dual-solo
 *
 * the parse process can be simply describe as
 * 1. rank count            --  count every rank in the card array
 * 2. rank count sort       --  sort the count result
 * 3. pattern match         --  iterate throw possible patterns
 */

#include "hand.h"

#define HAND_MIN_LENGTH             1
#define HAND_MAX_LENGTH             20
#define HAND_SOLO_CHAIN_MIN_LENGTH  5
#define HAND_PAIR_CHAIN_MIN_LENGTH  6
#define HAND_TRIO_CHAIN_MIN_LENGTH  6

/*
 * ************************************************************
 * pattern
 * ************************************************************
 */

#define PATTERN_LENGTH 12

#define HANDS_PATTERN_NONE  0   /* place holder */
#define HANDS_PATTERN_1     1   /* 1, solo */
#define HANDS_PATTERN_2     2   /* 2, pair/nuke */
#define HANDS_PATTERN_3     3   /* 3, trio */
#define HANDS_PATTERN_4_1   4   /* bomb */
#define HANDS_PATTERN_4_2   5   /* trio solo */
#define HANDS_PATTERN_5_1   6   /* solo chain */
#define HANDS_PATTERN_5_2   7   /* trio pair */
#define HANDS_PATTERN_6_1   8   /* solo chain */
#define HANDS_PATTERN_6_2   9   /* pair chain */
#define HANDS_PATTERN_6_3   10  /* trio chain */
#define HANDS_PATTERN_6_4   11  /* four dual solo */
#define HANDS_PATTERN_7_1   12  /* solo chain */
#define HANDS_PATTERN_8_1   13  /* solo chain */
#define HANDS_PATTERN_8_2   14  /* pair chain */
#define HANDS_PATTERN_8_3   15  /* trio solo chian */
#define HANDS_PATTERN_8_4   16  /* four dual pair */
#define HANDS_PATTERN_9_1   17  /* solo chain */
#define HANDS_PATTERN_9_2   18  /* trio chain */
#define HANDS_PATTERN_10_1  19  /* solo chain */
#define HANDS_PATTERN_10_2  20  /* pair chain */
#define HANDS_PATTERN_10_3  21  /* trio pair chain */
#define HANDS_PATTERN_11    22  /* solo chain */
#define HANDS_PATTERN_12_1  23  /* solo chain */
#define HANDS_PATTERN_12_2  24  /* pair chain */
#define HANDS_PATTERN_12_3  25  /* trio chain */
#define HANDS_PATTERN_12_4  26  /* trio solo chain */
#define HANDS_PATTERN_14    27  /* pair chain */
#define HANDS_PATTERN_16_1  28  /* pair chain */
#define HANDS_PATTERN_16_2  29  /* trio solo chain */
#define HANDS_PATTERN_18    30  /* pair chain */
#define HANDS_PATTERN_20_1  31  /* pair chain */
#define HANDS_PATTERN_20_2  32  /* trio solo chain */
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

/* sort count array by counts */
void _Hand_SortCount(int *count)
{
    qsort(count, CARD_RANK_END, sizeof(int), _Hand_PatternSort);
}

/* 
 * count rank in card array 
 * count[rank] = num
 */
void Hand_CountRank(card_array_t *array, int *count, int *sort)
{
    int i = 0;
    memset(count, 0, sizeof(int) * CARD_RANK_END);
    for (i = 0; i < array->length; i++)
        count[CARD_RANK(array->cards[i])]++;
    
    if (sort != NULL)
    {
        memcpy(sort, count, sizeof(int) * CARD_RANK_END);
        _Hand_SortCount(sort);
    }
}

/* check if a sorted count array matches specific pattern */
int _Hand_PatternMatch(int *sorted, int pattern)
{
    int i = 0;
    int ret = 1;
    
    for (i = 0; i < PATTERN_LENGTH; i++)
    {
        if (sorted[i] != _hand_pattern[pattern][i])
        {
            ret = 0;
            break;
        }
    }
    
    return ret;
}

/* 
 * check pattern as 334455 666777 etc
 * | 333 | 444 | 555 | 666 |
 * | 123 |                   num: 3
 * |  1     2     3     4  | expectLength: 4
 */
int _Hand_CheckChain(int *count, int num, int expectLength)
{
    int i = 0;
    int marker = 0;
    int length = 0;
    
    for (i = CARD_RANK_3; i < CARD_RANK_R; i++)
    {
        /* found first match */
        if (count[i] == num && marker == 0)
        {
            marker = i;
            continue;
        }
        
        /* matches end */
        if ((count[i] != num && marker != 0))
        {
            length = i - marker;
            break;
        }
    }
    
    return (length == expectLength ? 1 : 0);
}

/*
 * distribute cards
 * for example, (xxx, 88666644, 422, 4, 2, 8)
 * hand will be 66668844
 */
void _Hand_Distribute(hand_t *hand, card_array_t *array, int *count, int d1, int d2, int length)
{
    int i = 0;
    int num = 0;
    uint8_t card = 0;
    card_array_t temp;
    
    CardArray_Clear(&temp);
    
    for (i = 0; i < array->length; i++)
    {
        card = array->cards[i];
        num = count[CARD_RANK(card)];
        
        if (num == d1)
            CardArray_PushBack(hand->cards, card);
        else if (num == d2)
            CardArray_PushBack(&temp, card);
        
        if (hand->cards->length + temp.length >= length)
        {
            CardArray_Concate(hand->cards, &temp);
            break;
        }
    }
}

void _Hand_Parse_1(hand_t *hand, card_array_t *array, int *count, int *sorted)
{
    /* solo */
    CardArray_Copy(hand->cards, array);
    hand->type = HAND_PRIMAL_SOLO | HAND_KICKER_NONE | HAND_CHAIN_NONE;
}

void _Hand_Parse_2(hand_t *hand, card_array_t *array, int *count, int *sorted)
{
    /* pair or nuke */
    if (CARD_RANK(array->cards[0]) == CARD_RANK(array->cards[1]))
    {
        hand->type = CARD_RANK(array->cards[0]) == CARD_RANK_R ? HAND_PRIMAL_NUKE : HAND_PRIMAL_PAIR;
        CardArray_Copy(hand->cards, array);
    }
}

void _Hand_Parse_3(hand_t *hand, card_array_t *array, int *count, int *sorted)
{
    /* trio */
    uint8_t rank = CARD_RANK(array->cards[0]);
    if (CARD_RANK(array->cards[1]) == rank &&
        CARD_RANK(array->cards[2]) == rank)
    {
        CardArray_Copy(hand->cards, array);
        hand->type = HAND_PRIMAL_TRIO;
    }
}

void _Hand_Parse_4(hand_t *hand, card_array_t *array, int *count, int *sorted)
{
    int i = 0;
    int trio = 0;
    uint8_t kicker = 0;
    uint8_t rank = 0;
    
    /* trio solo | bomb */
    rank = CARD_RANK(array->cards[0]);
    if (CARD_RANK(array->cards[1]) == rank &&
        CARD_RANK(array->cards[2]) == rank &&
        CARD_RANK(array->cards[3]) == rank)
    {
        CardArray_Copy(hand->cards, array);
        hand->type = HAND_PRIMAL_BOMB;
    }
    else
    {
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

void _Hand_Parse_5(hand_t *hand, card_array_t *array, int *count, int *sorted)
{
    int i = 0;
    int j = 0;
    int trio = 0;
    int pair = 0;
    card_array_t pairArray;
    
    CardArray_Clear(&pairArray);
    
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
                    CardArray_PushBack(hand->cards, array->cards[j]);
                else
                    CardArray_PushBack(&pairArray, array->cards[j]);
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

void _Hand_Parse_6(hand_t *hand, card_array_t *array, int *count, int *sorted)
{
    if (_Hand_PatternMatch(sorted, HANDS_PATTERN_6_4))
    {
        /* four dual solo, 4-1-1 */
        _Hand_Distribute(hand, array, count, 4, 1, 6);
        hand->type = HAND_PRIMAL_FOUR | HAND_KICKER_DUAL_SOLO | HAND_CHAIN_NONE;
    }
}

/* length 7 only got solo chain avaliable, skip */
void _Hand_Parse_8(hand_t *hand, card_array_t *array, int *count, int *sorted)
{
    /* trio solo chian */
    if (_Hand_PatternMatch(sorted, HANDS_PATTERN_8_3))
    {
        if (_Hand_CheckChain(count, 3, 2))
        {
            _Hand_Distribute(hand, array, count, 3, 1, 8);
            hand->type = HAND_PRIMAL_TRIO | HAND_KICKER_SOLO | HAND_CHAIN;
        }
    }
    /* four dual pair, no joker */
    else if (_Hand_PatternMatch(sorted, HANDS_PATTERN_8_4) && count[CARD_RANK_R] == 0)
    {
        _Hand_Distribute(hand, array, count, 4, 2, 8);
        hand->type = HAND_PRIMAL_FOUR | HAND_KICKER_DUAL_PAIR | HAND_CHAIN_NONE;
    }
}

/* length 9 only got solo chain and trio chain avaliable, skip */
void _Hand_Parse_10(hand_t *hand, card_array_t *array, int *count, int *sorted)
{
    /* trio pair chain */
    if (_Hand_PatternMatch(sorted, HANDS_PATTERN_10_3))
    {
        /* trio pair chain, no joker */
        if (_Hand_CheckChain(count, 3, 2) && count[CARD_RANK_R] == 0)
        {
            _Hand_Distribute(hand, array, count, 3, 2, 10);
            hand->type = HAND_PRIMAL_TRIO | HAND_KICKER_PAIR | HAND_CHAIN;
        }
    }
}

/* length 11 only got solo chain avaliable, skip */
void _Hand_Parse_12(hand_t *hand, card_array_t *array, int *count, int *sorted)
{
    /* trio solo chain */
    if (_Hand_PatternMatch(sorted, HANDS_PATTERN_12_4))
    {
        if (_Hand_CheckChain(count, 3, 3))
        {
            _Hand_Distribute(hand, array, count, 3, 1, 12);
            hand->type = HAND_PRIMAL_TRIO | HAND_KICKER_SOLO | HAND_CHAIN;
        }
    }
}

/* length 13, 15 skipped, length 14 only go pair chain */
void _Hand_Parse_16(hand_t *hand, card_array_t *array, int *count, int *sorted)
{
    /* trio solo chain */
    if (_Hand_PatternMatch(sorted, HANDS_PATTERN_16_2))
    {
        if (_Hand_CheckChain(count, 3, 4))
        {
            _Hand_Distribute(hand, array, count, 3, 1, 16);
            hand->type = HAND_PRIMAL_TRIO | HAND_KICKER_SOLO | HAND_CHAIN;
        }
    }
}

/* 17, 19 skipped, 18 pair/trio chain only */
void _Hand_Parse_20(hand_t *hand, card_array_t *array, int *count, int *sorted)
{
    /* trio solo chain */
    if (_Hand_PatternMatch(sorted, HANDS_PATTERN_20_2))
    {
        if (_Hand_CheckChain(count, 3, 5))
        {
            _Hand_Distribute(hand, array, count, 3, 1, 20);
            hand->type = HAND_PRIMAL_TRIO | HAND_KICKER_SOLO | HAND_CHAIN;
        }
    }
}

void _Hand_Parse_Default(hand_t *hand, card_array_t *array, int *count, int *sorted)
{
    hand->type = 0;
}

typedef void (* Hand_Parser)(hand_t *, card_array_t *, int *, int *);

void Hand_Parse(hand_t *hand, card_array_t *array)
{
    int count[CARD_RANK_END];
    int sorted[CARD_RANK_END];
    Hand_Parser parser[HAND_MAX_LENGTH + 1];
    
    /* setup parser function table */
    parser[HAND_MIN_LENGTH + 0]     = _Hand_Parse_1;
    parser[HAND_MIN_LENGTH + 1]     = _Hand_Parse_2;
    parser[HAND_MIN_LENGTH + 2]     = _Hand_Parse_3;
    parser[HAND_MIN_LENGTH + 3]     = _Hand_Parse_4;
    parser[HAND_MIN_LENGTH + 4]     = _Hand_Parse_5;
    parser[HAND_MIN_LENGTH + 5]     = _Hand_Parse_6;
    parser[HAND_MIN_LENGTH + 6]     = _Hand_Parse_Default;
    parser[HAND_MIN_LENGTH + 7]     = _Hand_Parse_8;
    parser[HAND_MIN_LENGTH + 8]     = _Hand_Parse_Default;
    parser[HAND_MIN_LENGTH + 9]     = _Hand_Parse_10;
    parser[HAND_MIN_LENGTH + 10]    = _Hand_Parse_Default;
    parser[HAND_MIN_LENGTH + 11]    = _Hand_Parse_12;
    parser[HAND_MIN_LENGTH + 12]    = _Hand_Parse_Default;
    parser[HAND_MIN_LENGTH + 13]    = _Hand_Parse_Default;
    parser[HAND_MIN_LENGTH + 14]    = _Hand_Parse_Default;
    parser[HAND_MIN_LENGTH + 15]    = _Hand_Parse_16;
    parser[HAND_MIN_LENGTH + 16]    = _Hand_Parse_Default;
    parser[HAND_MIN_LENGTH + 17]    = _Hand_Parse_Default;
    parser[HAND_MIN_LENGTH + 18]    = _Hand_Parse_Default;
    parser[HAND_MIN_LENGTH + 19]    = _Hand_Parse_20;
    
    /* sort cards */
    CardArray_Sort(array, NULL);
    
    /* count ranks */
    Hand_CountRank(array, count, sorted);
    
    /* clear hand */
    Hand_Clear(hand);
    
    /* validate length */
    if (array->length < HAND_MIN_LENGTH || array->length > HAND_MAX_LENGTH)
    {
        hand->type = HAND_PRIMAL_NONE;
    }
    /* solo chain */
    else if (array->length >= HAND_SOLO_CHAIN_MIN_LENGTH && _Hand_CheckChain(count, 1, array->length))
    {
        hand->type = HAND_PRIMAL_SOLO | HAND_KICKER_NONE | HAND_CHAIN;
        CardArray_Copy(hand->cards, array);
    }
    /* pair chain */
    else if (array->length >= HAND_PAIR_CHAIN_MIN_LENGTH && array->length % 2 == 0 && _Hand_CheckChain(count, 2, array->length / 2))
    {
        hand->type = HAND_PRIMAL_PAIR | HAND_KICKER_NONE | HAND_CHAIN;
        CardArray_Copy(hand->cards, array);
    }
    /* trio chain */
    else if (array->length >= HAND_TRIO_CHAIN_MIN_LENGTH && array->length % 3 == 0 && _Hand_CheckChain(count, 3, array->length / 3))
    {
        hand->type = HAND_PRIMAL_TRIO | HAND_KICKER_NONE | HAND_CHAIN;
        CardArray_Copy(hand->cards, array);
    }
    /* other type */
    else
    {
        parser[array->length](hand, array, count, sorted);
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

void Hand_Print(hand_t *hand)
{
    int i = 0;
    char str[10];
    
    printf("hand: %p \n", (void *)hand);
    printf("type: ");
    switch (Hand_GetPrimal(hand->type))
    {
        case HAND_PRIMAL_NONE:
            printf("none ");
            break;
        case HAND_PRIMAL_SOLO:
            printf("solo ");
            break;
        case HAND_PRIMAL_PAIR:
            printf("pair ");
            break;
        case HAND_PRIMAL_TRIO:
            printf("trio ");
            break;
        case HAND_PRIMAL_FOUR:
            printf("four ");
            break;
        case HAND_PRIMAL_BOMB:
            printf("bomb ");
            break;
        case HAND_PRIMAL_NUKE:
            printf("nuke ");
            break;
        default:
            break;
    }
    
    switch (Hand_GetKicker(hand->type))
    {
        case HAND_KICKER_NONE:
            printf("none ");
            break;
        case HAND_KICKER_SOLO:
            printf("solo ");
            break;
        case HAND_KICKER_PAIR:
            printf("pair ");
            break;
        case HAND_KICKER_DUAL_SOLO:
            printf("dual solo ");
            break;
        case HAND_KICKER_DUAL_PAIR:
            printf("dual pair ");
            break;
        default:
            break;
    }
    
    switch (Hand_GetChain(hand->type))
    {
        case HAND_CHAIN:
            printf("chain ");
            break;
        default:
            break;
    }
    printf("\n");
    
    memset(str, 0, 10);
    printf("Cards: %d | ", hand->cards->length);
    
    for (i = 0; i < hand->cards->length; i++)
    {
        Card_ToString(hand->cards->cards[i], str, 10);
        printf("%s ", str);
    }
    printf("\n");
}

/*
 * ************************************************************
 * hand list
 * ************************************************************
 */

hand_list_t *HandList_Create(void)
{
    hand_list_t *hl = (hand_list_t *)calloc(1, sizeof(hand_list_t));
    return hl;
}

void HandList_PushFront(hand_list_t **hl, hand_t *hand)
{
    hand_list_t *head = *hl;
    if (head == NULL)
    {
        head = (hand_list_t *)calloc(1, sizeof(hand_list_t));
        head->hand = hand;
        head->next = NULL;
    }
    else if (head->hand == NULL)
    {
        head->hand = hand;
    }
    else
    {
        hand_list_t *newhead = (hand_list_t *)calloc(1, sizeof(hand_list_t));
        newhead->hand = hand;
        newhead->next = head;
        head = newhead;
    }
    *hl = head;
}

void HandList_Destroy(hand_list_t *hl)
{
    hand_list_t *temp;
    hand_list_t *prev;
    
    if (hl == NULL)
        return;
    
    for (temp = hl; temp != NULL; )
    {
        prev = temp;
        temp = temp->next;
        
        if (prev->hand != NULL)
            Hand_Destroy(prev->hand);
        
        free(prev);
    }
}

void _HandList_ExtractConsecutive_1(hand_list_t *hl, card_array_t *array)
{
    int i = 0;
    int j = 0;
    int cardnum = 0;
    uint8_t lastrank = 0;
    card_array_t *arrtemp = NULL;
    hand_t *hand = NULL;
    
    arrtemp = CardArray_CreateEmpty();
    
    cardnum = array->length;
    lastrank = array->cards[0];
    i = 1;
    while (cardnum--)
    {
        if ((lastrank + 1) != CARD_RANK(array->cards[i]))
        {
            /* chain break */
            if (i >= HAND_SOLO_CHAIN_MIN_LENGTH)
            {
                /* solo chian */
                hand = Hand_Create();
                hand->type = HAND_PRIMAL_SOLO | HAND_KICKER_NONE | HAND_CHAIN;
                for (j = 0; j < i; j++)
                {
                    CardArray_PushBack(hand->cards, CardArray_PopFront(array));
                }
            }
            else
            {
                /* not a solo chain */
                for (j = 0; j < i; j++)
                {
                    hand = Hand_Create();
                    hand->type = HAND_PRIMAL_SOLO | HAND_KICKER_NONE | HAND_CHAIN_NONE;
                    CardArray_PushBack(hand->cards, CardArray_PopFront(array));
                    HandList_PushFront(&hl, hand);
                }
            }
            
            lastrank = CARD_RANK(array->cards[0]);
            i = 1;
        }
        else
        {
            i++;
        }
    }
    
    CardArray_Destroy(arrtemp);
}

void _HandList_ExtractConsecutive(hand_list_t *hl, card_array_t *array, int duplicate)
{
    switch (duplicate)
    {
        case 1:
            _HandList_ExtractConsecutive_1(hl, array);
            break;
        case 2:
            break;
        case 3:
            break;
            
        default:
            break;
    }
}

hand_list_t *HandList_StandardAnalyze(card_array_t *array)
{
    int i = 0;
    int count[CARD_RANK_END];
    hand_list_t *hl = NULL;
    hand_t *hand = NULL;
    
    card_array_t *arrsolo = NULL;
    card_array_t *arrpair = NULL;
    card_array_t *arrtrio = NULL;
    
    arrsolo = CardArray_CreateEmpty();
    arrpair = CardArray_CreateEmpty();
    arrtrio = CardArray_CreateEmpty();
    
    CardArray_Sort(array, NULL);
    Hand_CountRank(array, count, NULL);
    
    /* nuke */
    if (count[CARD_RANK_R] == 2)
    {
        hand = Hand_Create();
        hand->type = HAND_PRIMAL_NUKE | HAND_KICKER_NONE | HAND_CHAIN_NONE;
        CardArray_TransferRank(hand->cards, array, CARD_RANK_R);
        
        HandList_PushFront(&hl, hand);
        
        count[CARD_RANK_R] = 0;
    }
    /* bomb */
    for (i = CARD_RANK_3; i < CARD_RANK_R; i++)
    {
        if (count[i] == 4)
        {
            hand = Hand_Create();
            hand->type = HAND_PRIMAL_BOMB | HAND_KICKER_NONE | HAND_CHAIN_NONE;
            CardArray_TransferRank(hand->cards, array, i);
            
            HandList_PushFront(&hl, hand);
            
            count[i] = 0;
        }
    }
    /* 2 */
    if (count[CARD_RANK_2] != 0)
    {
        hand = Hand_Create();
        for (i = 0; i < array->length; i++)
        {
            if (CARD_RANK(array->cards[i]) == CARD_RANK_2)
                CardArray_PushBack(hand->cards, array->cards[i]);
        }
        
        switch (count[CARD_RANK_2])
        {
            case 1:
                hand->type = HAND_PRIMAL_SOLO | HAND_KICKER_NONE | HAND_CHAIN_NONE;
                break;
            case 2:
                hand->type = HAND_PRIMAL_PAIR | HAND_KICKER_NONE | HAND_CHAIN_NONE;
                break;
            case 3:
                hand->type = HAND_PRIMAL_TRIO | HAND_KICKER_NONE | HAND_CHAIN_NONE;
            default:
                break;
        }
        
        HandList_PushFront(&hl, hand);
    }
    /* chains */
    for (i = CARD_RANK_3; i < CARD_RANK_2; i++)
    {
        switch (count[i])
        {
            case 1:
                CardArray_TransferRank(arrsolo, array, i);
                break;
            case 2:
                CardArray_TransferRank(arrpair, array, i);
                break;
            case 3:
                CardArray_TransferRank(arrtrio, array, i);
                break;
            default:
                break;
        }
    }
    
    /* chain */
    _HandList_ExtractConsecutive(hl, arrsolo, 1);
    _HandList_ExtractConsecutive(hl, arrpair, 2);
    _HandList_ExtractConsecutive(hl, arrpair, 3);
    
    CardArray_Destroy(arrsolo);
    CardArray_Destroy(arrpair);
    CardArray_Destroy(arrtrio);
    
    return hl;
}

hand_list_t *HandList_FindBeat(card_array_t *array, hand_t *beat)
{
    return NULL;
}

void HandList_Print(hand_list_t *hl)
{
    hand_list_t *node = hl;
    
    if (node == NULL || node->hand == NULL)
        return;
    
    printf("-----hand_list_t begin---------\n");
    for (node = hl; node != NULL; node = node->next)
    {
        Hand_Print(node->hand);
    }
    printf("-----hand_list_t ended---------\n");
}