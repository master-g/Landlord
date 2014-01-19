/*
 *  card.c
 *  Landlord
 *
 *  Created by Master.G on 14-1-17.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "card.h"
#include <string.h>
#include <time.h>

card_array_t *CardArray_CreateEmpty(void)
{
    card_array_t *array = (card_array_t *)malloc(sizeof(card_array_t));
    memset(array, 0, sizeof(card_array_t));
    
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

void CardArray_Reset(card_array_t *array)
{
    int rank = 0;
    
    /* 52 standard cards */
    for (rank = CARD_RANK_3; rank < CARD_RANK_R; rank++)
    {
        array->cards[rank - CARD_RANK_3 + 13 * 0] = Card_Make(CARD_SUIT_CLUB, rank);
        array->cards[rank - CARD_RANK_3 + 13 * 1] = Card_Make(CARD_SUIT_DIAMOND, rank);
        array->cards[rank - CARD_RANK_3 + 13 * 2] = Card_Make(CARD_SUIT_HEART, rank);
        array->cards[rank - CARD_RANK_3 + 13 * 3] = Card_Make(CARD_SUIT_SPADE, rank);
    }
    
    /* jokers */
    array->cards[CARD_SET_LENGTH - 2] = Card_Make(CARD_SUIT_CLUB, CARD_RANK_R);
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

uint8_t CardArray_PushBack(card_array_t *array, uint8_t card)
{
    uint8_t ret = 0;
    
    if (!CardArray_IsFull(array))
    {
        array->cards[array->length] = card;
        array->length++;
        
        ret = card;
    }
    
    return 0;
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
        array->length--;
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
        }
    }
    
    return ret;
}


#define CARD_STRING_LENGTH 4

char szDIAMOND[] = { 0xE2, 0x99, 0xA6, 0};
char szCLUB[]    = { 0xE2, 0x99, 0xA3, 0};
char szHEART[]   = { 0xE2, 0x99, 0xA5, 0};
char szSPADE[]   = { 0xE2, 0x99, 0xA0, 0};
char szRank[]    = { '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A', '2', 'R' };

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
    printf("CardArray: %p\nLength: %d\nCards: ", (void *)array, array->length);
    
    for (i = 0; i < array->length; i++)
    {
        Card_ToString(array->cards[i], str, 10);
        printf("%s ", str);
    }
    
    printf("\n");
}

/*
 * test functions
 */
#define PRINT_TEST_SEPARATOR printf("------------------------------\n")
#define MAKE_RANDOM_CARD Card_Make((rand() % 4 + 1) << 4, rand() % 13 + 1)

void CardArray_FillRandomCards(card_array_t *array, int length)
{
    int i = 0;
    for (i = 0; i < length; i++)
        CardArray_PushBack(array, MAKE_RANDOM_CARD);
}

void CardArray_Test(void)
{
    int i = 0;
    int j = 0;
    int card = 0;
    char str[10];
    card_array_t *arr = CardArray_CreateSet();
    card_array_t *tail = CardArray_CreateEmpty();
    
    srand((unsigned int)time(NULL));
    
    printf("Testing CardArray_CreateSet\n");
    PRINT_TEST_SEPARATOR;
    
    for (i = 0; i < CARD_SET_LENGTH; i++)
    {
        memset(str, 0, 10);
        Card_ToString(arr->cards[i], str, 10);
        printf("%s ", str);
        
        j++;
        if (j % 13 == 0)
            printf("\n");
    }
    
    printf("\n\nTesting CardArray_CreateEmpty\n");
    PRINT_TEST_SEPARATOR;
    CardArray_Destroy(arr);
    arr = CardArray_CreateEmpty();
    printf("\tarray length: %d\n", arr->length);
    printf("\tpushing elements...\n");
    CardArray_FillRandomCards(arr, 60);
    printf("\tafter pushing\n");
    CardArray_Print(arr);
    
    printf("\nTesting CardArray_Concate\n");
    PRINT_TEST_SEPARATOR;
    CardArray_Clear(arr);
    CardArray_Clear(tail);
    CardArray_FillRandomCards(arr, 3);
    CardArray_FillRandomCards(tail, 5);
    printf("\tarray 1:\n");
    CardArray_Print(arr);
    printf("\tarray 2:\n");
    CardArray_Print(tail);
    printf("\tafter concate:\n");
    CardArray_Concate(arr, tail);
    CardArray_Print(arr);
    printf("\nTesting CardArray_PushBack\n");
    PRINT_TEST_SEPARATOR;
    
    card = MAKE_RANDOM_CARD;
    Card_ToString(card, str, 10);
    printf("\tpushing card: %s\n", str);
    CardArray_PushBack(arr, card);
    card = MAKE_RANDOM_CARD;
    Card_ToString(card, str, 10);
    printf("\tpushing card: %s\n", str);
    CardArray_PushBack(arr, card);
    card = MAKE_RANDOM_CARD;
    Card_ToString(card, str, 10);
    printf("\tpushing card: %s\n", str);
    CardArray_PushBack(arr, card);
    printf("\tafter pushback\n");
    CardArray_Print(arr);
    
    printf("\nTesting CardArray_PushFront\n");
    PRINT_TEST_SEPARATOR;
    printf("\tbefore pushback\n");
    CardArray_Print(arr);
    card = MAKE_RANDOM_CARD;
    Card_ToString(card, str, 10);
    printf("\tpushing card: %s\n", str);
    CardArray_PushFront(arr, card);
    card = MAKE_RANDOM_CARD;
    Card_ToString(card, str, 10);
    printf("\tpushing card: %s\n", str);
    CardArray_PushFront(arr, card);
    card = MAKE_RANDOM_CARD;
    Card_ToString(card, str, 10);
    printf("\tpushing card: %s\n", str);
    CardArray_PushFront(arr, card);
    printf("\tafter pushfront\n");
    CardArray_Print(arr);
    
    printf("\nTesting CardArray_PopFront\n");
    PRINT_TEST_SEPARATOR;
    card = CardArray_PopFront(arr);
    Card_ToString(card, str, 10);
    printf("\tpoping card: %s\n", str);
    card = CardArray_PopFront(arr);
    Card_ToString(card, str, 10);
    printf("\tpoping card: %s\n", str);
    card = CardArray_PopFront(arr);
    Card_ToString(card, str, 10);
    printf("\tpoping card: %s\n", str);
    printf("\tafter pushfront\n");
    CardArray_Print(arr);
    
    printf("\nTesting CardArray_PopBack\n");
    PRINT_TEST_SEPARATOR;
    card = CardArray_PopBack(arr);
    Card_ToString(card, str, 10);
    printf("\tpoping card: %s\n", str);
    card = CardArray_PopBack(arr);
    Card_ToString(card, str, 10);
    printf("\tpoping card: %s\n", str);
    card = CardArray_PopBack(arr);
    Card_ToString(card, str, 10);
    printf("\tpoping card: %s\n", str);
    printf("\tafter popback\n");
    CardArray_Print(arr);
    
    printf("\nTesting CardArray_Insert\n");
    PRINT_TEST_SEPARATOR;
    card = MAKE_RANDOM_CARD;
    Card_ToString(card, str, 10);
    i = rand() % arr->length;
    card = CardArray_Insert(arr, i, card);
    printf("\tinsert %s berfore %d\n", str, i);
    card = MAKE_RANDOM_CARD;
    Card_ToString(card, str, 10);
    i = rand() % arr->length;
    card = CardArray_Insert(arr, i, card);
    printf("\tinsert %s berfore %d\n", str, i);
    card = MAKE_RANDOM_CARD;
    Card_ToString(card, str, 10);
    i = rand() % arr->length;
    card = CardArray_Insert(arr, i, card);
    printf("\tinsert %s berfore %d\n", str, i);
    printf("\tafter insert\n");
    CardArray_Print(arr);
    
    printf("\nTesting CardArray_Remove\n");
    PRINT_TEST_SEPARATOR;
    i = rand() % arr->length;
    card = CardArray_Remove(arr, i);
    Card_ToString(card, str, 10);
    printf("\tremove %s at %d\n", str, i);
    i = rand() % arr->length;
    card = CardArray_Remove(arr, i);
    Card_ToString(card, str, 10);
    printf("\tremove %s at %d\n", str, i);
    i = rand() % arr->length;
    card = CardArray_Remove(arr, i);
    Card_ToString(card, str, 10);
    printf("\tremove %s at %d\n", str, i);
    printf("\tafter remove\n");
    CardArray_Print(arr);
    
    CardArray_Destroy(arr);
    CardArray_Destroy(tail);
    
}
