/*
 *  main.c
 *  Landlord
 *
 *  Created by Master.G on 14-1-19.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "common.h"
#include "game.h"

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

void silent_printf(const char *fmt, ...)
{
    
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
    
    printf("peasant : %d\n", peasantwon);
    printf("landlord : %d\n", landlordwon);
    
    Game_Clear(&game);
    
    printf("ended at %ld\n", time(NULL));

    memtrack_list_allocations();

    return 0;
}
