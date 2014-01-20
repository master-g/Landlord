/*
 *  main.c
 *  Landlord
 *
 *  Created by Master.G on 14-1-19.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "common.h"
#include "deck.h"

int main(int argc, const char * argv[])
{
    int i = 0;
    deck_t *deck = NULL;
    card_array_t *hands[3];
    
    deck = Deck_Create();
    Deck_Shuffle(deck, NULL);
    
    for (i = 0; i < 3; i++)
    {
        hands[i] = CardArray_CreateEmpty();
        Deck_Deal(deck, hands[i], 17);
        CardArray_Sort(hands[i], NULL);
        CardArray_Print(hands[i]);
    }
    
    for (i = 0; i < 3; i++)
        CardArray_Destroy(hands[i]);
    
    Deck_Destroy(deck);
    
    memtrack_list_allocations();
    return 0;
}
