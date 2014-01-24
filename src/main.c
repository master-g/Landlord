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
#include "mt19937.h"

/* 
 * TODO
 * parse string to card array
 */

void test_hand(void)
{
    int i = 0;
    deck_t *deck = NULL;
    card_array_t *cards;
    hand_t *hand;
    mt19937_t mt;
    
    memset(&hand, 0, sizeof(hand));
    
    Random_Init(&mt, (uint32_t)time(NULL));
    
    hand = Hand_Create();
    deck = Deck_Create();
    cards = CardArray_CreateEmpty();
    
#define shit2
    
#ifdef shit
    CardArray_PushBack(cards, Card_Make(CARD_SUIT_CLUB, CARD_RANK_3));
    CardArray_PushBack(cards, Card_Make(CARD_SUIT_DIAMOND, CARD_RANK_3));
    CardArray_PushBack(cards, Card_Make(CARD_SUIT_HEART, CARD_RANK_3));
    CardArray_PushBack(cards, Card_Make(CARD_SUIT_CLUB, CARD_RANK_4));
    CardArray_PushBack(cards, Card_Make(CARD_SUIT_DIAMOND, CARD_RANK_4));
    CardArray_PushBack(cards, Card_Make(CARD_SUIT_HEART, CARD_RANK_4));
    CardArray_PushBack(cards, Card_Make(CARD_SUIT_CLUB, CARD_RANK_5));
    CardArray_PushBack(cards, Card_Make(CARD_SUIT_DIAMOND, CARD_RANK_5));
    CardArray_PushBack(cards, Card_Make(CARD_SUIT_HEART, CARD_RANK_5));
    CardArray_PushBack(cards, Card_Make(CARD_SUIT_CLUB, CARD_RANK_6));
    CardArray_PushBack(cards, Card_Make(CARD_SUIT_DIAMOND, CARD_RANK_6));
    CardArray_PushBack(cards, Card_Make(CARD_SUIT_HEART, CARD_RANK_6));
    
    Hand_Parse(hand, cards);
    
    if (hand->type != 0)
    {
        Hand_Print(hand);
        printf("--------------------\n");
    }
#else
    while (1)
    {
        Deck_Reset(deck);
        Deck_Shuffle(deck, &mt);
        
        CardArray_Clear(cards);
        
        Deck_Deal(deck, cards, 20);
    
        Hand_Parse(hand, cards);
        
        if (hand->type != 0)
        {
            Hand_Print(hand);
            printf("=====================\n");
        }
    }
#endif
    
    Hand_Destroy(hand);
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
