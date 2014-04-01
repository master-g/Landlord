//
//  main.cpp
//  llcpp
//
//  Created by Master.G on 14-4-1.
//  Copyright (c) 2014年 MED. All rights reserved.
//

#include <iostream>
#include "medlandlord.hpp"

int main(int argc, const char * argv[])
{
    int peasantwon = 0;
    int landlordwon = 0;
    int i = 0;
    
    game_t game;
    
    silent_printf("warning suppressor", i);
    
    HandList_Print(NULL);
    
    std::cout << "start at " << time(NULL) << std::endl;
    
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
    
    std::cout << "peasants : " << peasantwon << std::endl;
    std::cout << "landlord : " << landlordwon << std::endl;
    
    Game_Clear(&game);
    
    std::cout << "ended at " << time(NULL) << std::endl;
    
    /*
     test_advanced_hand_analyzer();
     */
    
    std::cout << std::endl;
    
    /*
     memtrack_list_allocations();
     */
    
    return 0;
}

