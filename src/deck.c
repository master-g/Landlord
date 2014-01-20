/*
 *  deck.c
 *  Landlord
 *
 *  Created by Master.G on 14-1-20.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "deck.h"

deck_t *Deck_Create(void)
{
    deck_t *deck = (deck_t *)calloc(1, sizeof(deck_t));
    deck->cards = CardArray_CreateSet();
    deck->used = CardArray_CreateEmpty();
    
    return deck;
}

void Deck_Destroy(deck_t *deck)
{
    CardArray_Destroy(deck->cards);
    CardArray_Destroy(deck->used);
    free(deck);
}

void shuffle(uint8_t arr[], int len, mt19937_t *mt)
{
	int i = len, j;
    uint8_t tmp = 0;
	
	while (--i > 0)
	{
        if (mt != NULL)
            j = Random_int32(mt) % (i+1);
        else
            j = rand() % (i+1);
		
        tmp = arr[j];
        arr[j] = arr[i];
        arr[i] = tmp;
	}
}

void Deck_Shuffle(deck_t *deck, mt19937_t *mtctx)
{
    shuffle(deck->cards->cards, deck->cards->length, mtctx);
}

void Deck_Reset(deck_t *deck)
{
    CardArray_Reset(deck->cards);
    CardArray_Clear(deck->used);
}

uint8_t Deck_DealSingle(deck_t *deck)
{
    return CardArray_PopBack(deck->cards);
}

void Deck_RecycleSingle(deck_t *deck, uint8_t card)
{
    CardArray_PushBack(deck->used, card);
}

int Deck_Deal(deck_t *deck, card_array_t *array, int count)
{
#ifdef DECK_PEDANTIC
    int i = 0;
    uint8_t card = 0;
    
    CardArray_Clear(array);
    
    for (i = 0; i < count; i++)
    {
        card = Deck_DealSingle(deck);
        if (card == 0)
            break;
        
        CardArray_PushBack(array, card);
    }
    
    return i;
#else
    int actualDealt = 0;
    
    CardArray_Clear(array);
    
    actualDealt = deck->cards->length >= count ? count : deck->cards->length;

    deck->cards->length -= actualDealt;
    memcpy(array->cards, &deck->cards->cards[deck->cards->length], actualDealt);
    array->length = actualDealt;
    
    return actualDealt;
#endif /* DEAL_PEDANTIC */
}

int Deck_Recycle(deck_t *deck, card_array_t *array)
{
#ifdef DECK_PEDANTIC
    int i = 0;
    uint8_t card = 0;
    
    for (i = 0; i < array->length; i++)
    {
        card = CardArray_PopBack(array);
        if (card == 0)
            break;
        
        Deck_RecycleSingle(deck, card);
    }
    
    return i;
#else
    int actualRecycled = 0;
    
    actualRecycled = CardArray_Capacity(deck->used) < array->length ? CardArray_Capacity(deck->used) : array->length;
    
    memcpy(&deck->used->cards[deck->used->length], array->cards, actualRecycled);
    deck->used->length += actualRecycled;
    
    return actualRecycled;
#endif /* DECK_PEDANTIC */
}

/*
 * test functions
 */
void Deck_Test(void)
{
    uint8_t card = 0;
    deck_t *deck;
    char str[10];
    card_array_t arr;
    
    deck = Deck_Create();
    srand((unsigned int)time(NULL));
    memset(str, 0, 10);
    
    printf("Testing Deck_Shuffle\n");
    PRINT_TEST_SEPARATOR;
    Deck_Shuffle(deck, NULL);
    printf("\tdeck->cards:\n");
    CardArray_Print(deck->cards);
    
    printf("Testing Deck_Reset\n");
    PRINT_TEST_SEPARATOR;
    Deck_Reset(deck);
    printf("\tdeck->cards:\n");
    CardArray_Print(deck->cards);
    
    printf("Testing Deck_DealSingle\n");
    PRINT_TEST_SEPARATOR;
    card = Deck_DealSingle(deck);
    Card_ToString(card, str, 10);
    printf("\tdeck deal: %s\n", str);
    
    printf("Testing Deck_RecycleSingle\n");
    PRINT_TEST_SEPARATOR;
    Deck_RecycleSingle(deck, card);
    printf("\tdeck after deal and recycle single\n");
    CardArray_Print(deck->cards);
    CardArray_Print(deck->used);
    
    printf("Testing Deck_Deal\n");
    PRINT_TEST_SEPARATOR;
    Deck_Deal(deck, &arr, rand()%10 + 5);
    printf("\tdealt cards\n");
    CardArray_Print(&arr);
    printf("\tdeck after deal\n");
    CardArray_Print(deck->cards);
    
    printf("Testing Deck_Recycle\n");
    PRINT_TEST_SEPARATOR;
    Deck_Recycle(deck, &arr);
    printf("\tafter recycle\n");
    CardArray_Print(deck->used);
    
    Deck_Destroy(deck);
}
