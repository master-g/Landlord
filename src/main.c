/*
 *  main.c
 *  Landlord
 *
 *  Created by Master.G on 14-1-19.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "common.h"
#include "deck.h"
#include "hand.h"

void test_hand(void)
{
    deck_t *deck = NULL;
    card_array_t *cards;
    hand_t hand;
    memset(&hand, 0, sizeof(hand));
    
    deck = Deck_Create();
    Deck_Shuffle(deck, NULL);
    
    cards = CardArray_CreateEmpty();
    Deck_Deal(deck, cards, rand() % 20 + 1);
    
    CardArray_Print(cards);
    
    CardArray_Destroy(cards);
    Deck_Destroy(deck);
}

int main(int argc, const char * argv[])
{
    /*
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
     */
    
    srand((unsigned int)time(NULL));
    test_hand();
    
    memtrack_list_allocations();
    return 0;
}
