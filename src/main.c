/*
 *  main.c
 *  Landlord
 *
 *  Created by Master.G on 14-1-19.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "common.h"
#include "game.h"
#include "medalgo.h"

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

void test_advanced_hand_analyzer()
{
    /*
    const char* str = "♣3 ♣4 ♠5 ♦6 ♠6 ♥7 ♠7 ♦7 ♦8 ♣8 ♣9 ♦9 ♦T";
    const char* str = "♣r ♣2 ♠K ♦K ♠Q ♥J ♠T ♥T ♦T ♣T ♠9 ♥9 ♦8 ♠7 ♥6 ♣6 ♠5 ♦5 ♠4 ♣4";
    const char* str = "♠K ♠Q ♥J ♠9 ♥9 ♦8 ♠7 ♥6 ♣6 ♠5 ♦5";
     */
    const char* str = "♠9 ♦8 ♠7 ♥6 ♠5 ♣4";
    card_array_t cards;
    medlist_t *hl;
    CardArray_InitFromString(&cards, str);
    
    CardArray_Print(&cards);
    
    hl = HandList_AdvancedAnalyze(&cards);
    
    HandList_Print(hl);

    HandList_Destroy(&hl);

    {
        hand_t ass;
        Hand_Parse(&ass, &cards);
        Hand_Print(&ass);
    }
}

int main(int argc, const char * argv[])
{
    int peasantwon = 0;
    int landlordwon = 0;
    int i = 0;
    
    game_t game;

    silent_printf("warning suppressor", i);

    printf("start at %ld\n", time(NULL));
        
    Game_Init(&game);
    
    for (i = 100000; i < 200000; i++)
    {
        Game_Play(&game, i);
        
        if (game.winner == game.landlord)
            landlordwon++;
        else
            peasantwon++;
        
        Game_Reset(&game);
    }
    
    printf("peasants : %d\n", peasantwon);
    printf("landlord : %d\n", landlordwon);
    
    Game_Clear(&game);
    
    printf("ended at %ld\n", time(NULL));
    
    /*
    test_advanced_hand_analyzer();
     */
    
    printf("\n");
    
    /*
    memtrack_list_allocations();
     */

    return 0;
}
