/*
 *  card.c
 *  Landlord
 *
 *  Created by Master.G on 14-1-17.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "card.h"

card_array_t *CardArray_CreateEmpty(void)
{
    card_array_t *array = (card_array_t *)calloc(1, sizeof(card_array_t));
    
    return array;
}

card_array_t *CardArray_CreateSet(void)
{
    card_array_t *array = CardArray_CreateEmpty();
    CardArray_Reset(array);
    
    return array;
}

void CardArray_Destroy(card_array_t *array)
{
    free(array);
}

card_array_t *CardArray_CreateFromString(const char *str)
{
    uint8_t card = 0;
    card_array_t *array = NULL;
    const char *p = str;
    
    array = CardArray_CreateEmpty();
    
    while (*p != '\0')
    {
        switch ((uint8_t)*p)
        {
            case 0xA6:
                card |= CARD_SUIT_DIAMOND;
                break;
            case 0xA3:
                card |= CARD_SUIT_CLUB;
                break;
            case 0xA5:
                card |= CARD_SUIT_HEART;
                break;
            case 0xA0:
                card |= CARD_SUIT_SPADE;
                break;
            case 'T':
                card |= CARD_RANK_T;
                break;
            case 'J':
                card |= CARD_RANK_J;
                break;
            case 'Q':
                card |= CARD_RANK_Q;
                break;
            case 'K':
                card |= CARD_RANK_K;
                break;
            case 'A':
                card |= CARD_RANK_A;
                break;
            case '2':
                card |= CARD_RANK_2;
                break;
            case 'r':
                card |= CARD_RANK_r;
                break;
            case 'R':
                card |= CARD_RANK_R;
                break;
            default:
                break;
        }
        
        if (*p <= '9' && *p >= '3')
            card |= CARD_RANK_3 + (uint8_t)(*p) - '3';
        
        if (CARD_RANK(card) != 0 && CARD_SUIT(card) != 0)
        {
            CardArray_PushBack(array, card);
            card = 0;
        }
        
        p++;
    }
    
    return array;
}

void CardArray_Reset(card_array_t *array)
{
    int rank = 0;
    
    /* 52 standard cards */
    for (rank = CARD_RANK_3; rank < CARD_RANK_r; rank++)
    {
        array->cards[rank - CARD_RANK_3 + 13 * 0] = Card_Make(CARD_SUIT_CLUB, rank);
        array->cards[rank - CARD_RANK_3 + 13 * 1] = Card_Make(CARD_SUIT_DIAMOND, rank);
        array->cards[rank - CARD_RANK_3 + 13 * 2] = Card_Make(CARD_SUIT_HEART, rank);
        array->cards[rank - CARD_RANK_3 + 13 * 3] = Card_Make(CARD_SUIT_SPADE, rank);
    }
    
    /* jokers */
    array->cards[CARD_SET_LENGTH - 2] = Card_Make(CARD_SUIT_CLUB, CARD_RANK_r);
    array->cards[CARD_SET_LENGTH - 1] = Card_Make(CARD_SUIT_DIAMOND, CARD_RANK_R);
    
    array->length = CARD_SET_LENGTH;
}

int CardArray_Concate(card_array_t *head, card_array_t *tail)
{
    int length = 0;
    int slot = 0;
    
    slot = CARD_SET_LENGTH - head->length;
    
    if (!CardArray_IsEmpty(tail) && slot > 0)
    {
        length = slot >= tail->length ? tail->length : slot;
        memcpy(&head->cards[head->length], tail->cards, length);
        head->length += length;
    }
    else
    {
        length = 0;
    }
    
    return length;
}

void CardArray_Subtract(card_array_t *from, card_array_t *sub)
{
    int i = 0;
    int j = 0;
    uint8_t card = 0;
    card_array_t temp;
    CardArray_Clear(&temp);
    
    for (i = 0; i < from->length; i++)
    {
        card = from->cards[i];
        for (j = 0; j < sub->length; j++)
        {
            if (card == sub->cards[j])
            {
                card = 0;
                break;
            }
        }
        
        if (card != 0)
            CardArray_PushBack(&temp, card);
    }
    
    CardArray_Copy(from, &temp);
}

int CardArray_IsIdentity(card_array_t *a, card_array_t *b)
{
    int i = 0;
    int identity = 0;
    card_array_t ta, tb;
    
    if (a == b)
        return 0;
    
    identity = a->length - b->length;
    if (identity == 0)
    {
        CardArray_Copy(&ta, a);
        CardArray_Copy(&tb, b);
        CardArray_Sort(&ta, NULL);
        CardArray_Sort(&tb, NULL);
        
        for (i = 0; i < a->length; i++)
        {
            if (ta.cards[i] != tb.cards[i])
            {
                identity = -1;
                break;
            }
        }
    }
    
    return identity;
}

int CardArray_IsContain(card_array_t *array, card_array_t *segment)
{
    int contain = 0;
    int i = 0;
    int j = 0;
    card_array_t temp;
    
    if (array->length == 0 || segment->length == 0)
        return contain;
    
    CardArray_Copy(&temp, segment);
    
    if (array->length >= segment->length)
    {
        for (i = 0; i < array->length; i++)
        {
            for (j = 0; j < temp.length; j++)
            {
                if (array->cards[i] == temp.cards[j])
                {
                    CardArray_RemoveCard(&temp, temp.cards[j]);
                    break;
                }
            }
        }
        
        contain = temp.length == 0 ? 1 : 0;
    }
    
    return contain;
}

uint8_t CardArray_PushBack(card_array_t *array, uint8_t card)
{
    uint8_t ret = 0;
    
    if (!CardArray_IsFull(array))
    {
        array->cards[array->length] = card;
        array->length++;
        
        ret = card;
    }
    
    return ret;
}

uint8_t CardArray_PushFront(card_array_t *array, uint8_t card)
{
    uint8_t ret = 0;
    
    if (!CardArray_IsFull(array))
    {
        memmove(array->cards + 1, array->cards, array->length);
        array->cards[0] = card;
        
        array->length++;
        
        ret = card;
    }
    
    return ret;
}

uint8_t CardArray_PopFront(card_array_t *array)
{
    uint8_t card = 0;
    
    if (!CardArray_IsEmpty(array))
    {
        card = array->cards[0];
        memmove(array->cards, array->cards + 1, array->length);
        array->cards[array->length--] = 0;
    }
    
    return card;
}

uint8_t CardArray_PopBack(card_array_t *array)
{
    uint8_t card = 0;
    
    if (!CardArray_IsEmpty(array))
    {
        card = array->cards[array->length - 1];
        array->cards[array->length - 1] = 0;
        array->length--;
    }
    
    return card;
}

uint8_t CardArray_Insert(card_array_t *array, int before, uint8_t card)
{
    uint8_t ret = 0;
    
    if (!CardArray_IsFull(array))
    {
        if (before == 0)
        {
            ret = CardArray_PushFront(array, card);
        }
        else if (before == array->length)
        {
            ret = CardArray_PushBack(array, card);
        }
        else if (before > 0 && before < array->length)
        {
            memmove(array->cards + before + 1, array->cards + before, array->length - before);
            array->cards[before] = card;
            array->length++;
            ret = card;
        }
    }
    
    return ret;
}

uint8_t CardArray_Remove(card_array_t *array, int where)
{
    uint8_t ret = 0;
    
    if (!CardArray_IsEmpty(array))
    {
        if (where == 0)
        {
            ret = CardArray_PopFront(array);
        }
        else if (where == array->length - 1)
        {
            ret = CardArray_PopBack(array);
        }
        else if (where > 0 && where < array->length - 1)
        {
            ret = array->cards[where];
            array->length--;
            memmove(array->cards + where, array->cards + where + 1, array->length - where);
            array->cards[array->length] = 0;
        }
    }
    
    return ret;
}

uint8_t CardArray_RemoveCard(card_array_t *array, uint8_t card)
{
    uint8_t ret = 0;
    int i = 0;
    
    for (i = 0; i < array->length; i++)
    {
        if (array->cards[i] == card)
        {
            ret = card;
            CardArray_Remove(array, i);
            break;
        }
    }
    
    return ret;
}

int CardArray_PushBackCards(card_array_t *array, card_array_t *from, int where, int count)
{
    int cards = 0;
    int i = 0;
    
    for (i = 0; i < count; i++)
    {
        CardArray_PushBack(array, from->cards[where + i]);
        cards++;
    }
    
    return cards;
}

void CardArray_CopyRank(card_array_t *dst, card_array_t *src, uint8_t rank)
{
    int i = 0;
    
    for (i = 0; i < src->length; i++)
    {
        if (CARD_RANK(src->cards[i]) == rank)
            CardArray_PushBack(dst, src->cards[i]);
    }
}

void CardArray_RemoveRank(card_array_t *array, uint8_t rank)
{
    int i = 0;
    
    card_array_t temp;
    CardArray_Clear(&temp);
    
    for (i = 0; i < array->length; i++)
    {
        if (CARD_RANK(array->cards[i]) != rank)
            CardArray_PushBack(&temp, array->cards[i]);
    }
    
    CardArray_Copy(array, &temp);
}

int CardArray_StandardSort(const void *a, const void *b)
{
    if (CARD_RANK(*(uint8_t *)a) == CARD_RANK(*(uint8_t *)b))
        return CARD_SUIT(*(uint8_t *)b) - CARD_SUIT(*(uint8_t *)a);
    else
        return CARD_RANK(*(uint8_t *)b) - CARD_RANK(*(uint8_t *)a);
}

void CardArray_Sort(card_array_t *array, int (*comparator)(const void *, const void *))
{
    if (comparator == NULL)
        qsort(array->cards, array->length, sizeof(uint8_t), CardArray_StandardSort);
    else
        qsort(array->cards, array->length, sizeof(uint8_t), comparator);
}


#define CARD_STRING_LENGTH 4

char szDIAMOND[] = { 0xE2, 0x99, 0xA6, 0};
char szCLUB[]    = { 0xE2, 0x99, 0xA3, 0};
char szHEART[]   = { 0xE2, 0x99, 0xA5, 0};
char szSPADE[]   = { 0xE2, 0x99, 0xA0, 0};
char szRank[]    = { '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A', '2', 'r', 'R' };

int Card_ToString(uint8_t card, char *buf, int len)
{
    if (buf != NULL && len >= CARD_STRING_LENGTH)
    {
        char *szSuit = NULL;
        int rank = CARD_RANK(card);
        int suit = CARD_SUIT(card);
        
        switch (suit)
        {
            case CARD_SUIT_DIAMOND:
                szSuit = szDIAMOND;
                break;
            case CARD_SUIT_CLUB:
                szSuit = szCLUB;
                break;
            case CARD_SUIT_HEART:
                szSuit = szHEART;
                break;
            case CARD_SUIT_SPADE:
                szSuit = szSPADE;
                break;
                
            default:
                szSuit = "ERR";
                break;
        }
        memcpy(buf, szSuit, CARD_STRING_LENGTH - 1);
        buf[CARD_STRING_LENGTH - 1] = szRank[rank - CARD_RANK_3];
    }
    
    return CARD_STRING_LENGTH;
}

void CardArray_Print(card_array_t *array)
{
    int i = 0;
    char str[10];
    memset(str, 0, 10);
#ifdef PRINT_ADDRESS
    DBGLog("Cards: %p (%d): ", (void *)array, array->length);
#else
    DBGLog("Cards: (%d): ", array->length);
#endif
    
    for (i = 0; i < array->length; i++)
    {
        Card_ToString(array->cards[i], str, 10);
        DBGLog("%s ", str);
    }
    
    DBGLog("\n");
}
