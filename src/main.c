/*
 *  main.c
 *  Landlord
 *
 *  Created by Master.G on 14-1-19.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "common.h"
#include "game.h"

void test_hand(void)
{
    deck_t *deck = NULL;
    card_array_t *cards;
    hand_t *hand;
    mt19937_t mt;
    
    memset(&hand, 0, sizeof(hand));
    
    Random_Init(&mt, (uint32_t)time(NULL));
    
    hand = Hand_Create();
    deck = Deck_Create();
    cards = CardArray_CreateEmpty();
    
#define testmode2
    
#ifdef testmode
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
        
        Deck_Deal(deck, cards, 5);
    
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

char szr[] = { '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A', '2', 'R' };

void Count_Print(int *count)
{
    int i = 0;
    for (i = CARD_RANK_BEG; i < CARD_RANK_END; i++)
    {
        printf("%c ", szr[i - 1]);
    }
    printf("\n");
    for (i = CARD_RANK_BEG; i < CARD_RANK_END; i++)
    {
        printf("%d ", count[i]);
    }
    printf("\n");
}

void test_code(void)
{
    int i = 0;
    deck_t *deck = NULL;
    card_array_t *hands[3];
    int count[CARD_RANK_END];
    hand_list_t *hl = NULL;
    
    srand((unsigned int)15);
    deck = Deck_Create();
    Deck_Shuffle(deck, NULL);
    
    for (i = 0; i < 3; i++)
    {
        hands[i] = CardArray_CreateEmpty();
        Deck_Deal(deck, hands[i], 17);
        CardArray_Sort(hands[i], NULL);
        Hand_CountRank(hands[i], count, NULL);
        CardArray_Print(hands[i]);
        Count_Print(count);
    }
    
    printf("\n");
    hl = HandList_StandardAnalyze(hands[2]);
    HandList_Print(hl);
    HandList_Destroy(hl);
    CardArray_Print(hands[2]);
    
    for (i = 0; i < 3; i++)
        CardArray_Destroy(hands[i]);
    
    Deck_Destroy(deck);
    /*
     srand((unsigned int)time(NULL));
     test_hand();
     */
}

int main(int argc, const char * argv[])
{
    int peasantwon = 0;
    int landlordwon = 0;
    int i = 0;
    
    game_t *game = NULL;
    
    printf("start at %ld\n", time(NULL));
        
    game = Game_Create();
    
    for (i = 1000000; i < 1000001; i++)
    {
        Game_Play(game, i);
        
        if (game->winner == game->landlord)
            landlordwon++;
        else
            peasantwon++;
        
        Game_Reset(game);
    }
    
    printf("peasant : %d\n", peasantwon);
    printf("landlord : %d\n", landlordwon);
    printf("shit : %d\n", shit);
    
    Game_Destroy(game);
    
    printf("ended at %ld\n", time(NULL));

    /*
    memtrack_list_allocations();
     */
    return 0;
}
