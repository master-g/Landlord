/*
 *  deck.c
 *  Landlord
 *
 *  Created by Master.G on 14-1-20.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "deck.h"
#include "mt19937.h"

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

void Deck_Shuffle(deck_t *deck, void *mtctx)
{
    shuffle(deck->cards.cards, deck->cards.length, (mt19937_t *)mtctx);
}

void Deck_Reset(deck_t *deck)
{
    CardArray_Reset(&deck->cards);
    CardArray_Clear(&deck->used);
}

uint8_t Deck_DealSingle(deck_t *deck)
{
    return CardArray_PopBack(&deck->cards);
}

void Deck_RecycleSingle(deck_t *deck, uint8_t card)
{
    CardArray_PushBack(&deck->used, card);
}

int Deck_Deal(deck_t *deck, card_array_t *array, int count)
{
#ifdef NICE_AND_CLEAN
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
    
    actualDealt = deck->cards.length >= count ? count : deck->cards.length;

    deck->cards.length -= actualDealt;
    memcpy(array->cards, &deck->cards.cards[deck->cards.length], actualDealt);
    array->length = actualDealt;
    
    return actualDealt;
#endif /* DEAL_PEDANTIC */
}

int Deck_Recycle(deck_t *deck, card_array_t *array)
{
#ifdef NICE_AND_CLEAN
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
    
    actualRecycled = CardArray_Capacity(&deck->used) < array->length ? CardArray_Capacity(&deck->used) : array->length;
    
    memcpy(&deck->used.cards[deck->used.length], array->cards, actualRecycled);
    deck->used.length += actualRecycled;
    
    return actualRecycled;
#endif /* DECK_PEDANTIC */
}
