/*
 *  hand.c
 *  Landlord
 *
 *  Created by Master.G on 14-1-20.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "hand.h"

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

uint8_t Hand_Parse(hand_t *hand, card_array_t *array)
{
    return 0;
}

int Hand_Compare(hand_t *a, hand_t *b)
{
    if (a->cards->length != b->cards->length)
    {
        
    }
    
    return 0;
}
