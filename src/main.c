/*
The MIT License (MIT)

Copyright (c) 2014 Master.G

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
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
    const char* str = "s9 d8 s7 h6 s5 c4";
     */
    const char* str = "♣3 ♣4 ♠5 ♦6 ♠6 ♥7 ♠7 ♦7 ♦8 ♣8 ♣9 ♦9 ♦T";
    card_array_t cards;
    medlist_t *hl;
    CardArray_InitFromString(&cards, str);
    
    CardArray_Print(&cards);
    
    hl = HandList_AdvancedAnalyze(&cards);
    
    HandList_Print(hl);

    HandList_Destroy(&hl);
}

void test_game()
{
    int peasantwon = 0;
    int landlordwon = 0;
    int i = 0;
    
    game_t game;
    
    printf("start at %ld\n", time(NULL));
    
    Game_Init(&game);
    
    for (i = 10000; i < 20000; i++)
    {
        Game_Play(&game, i);
        
        if (game.winner == game.landlord)
            landlordwon++;
        else
            peasantwon++;
        
        Game_Reset(&game);
        
        if (i % 10000 == 0)
        {
            printf("%.1f%%\n", (i - 10000) / 10000.0f * 100.0f);
        }
    }
    
    printf("peasants : %d\n", peasantwon);
    printf("landlord : %d\n", landlordwon);
    
    Game_Clear(&game);
    
    printf("ended at %ld\n", time(NULL));
    
    printf("\n");
    
    
}

int main(int argc, const char * argv[])
{
    test_game();
    /* test_advanced_hand_analyzer(); */
    memtrack_list_allocations();
    return 0;
}
