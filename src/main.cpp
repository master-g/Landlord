//
//  main.cpp
//  llcpp
//
//  Created by Master.G on 14-4-1.
//  Copyright (c) 2014 MED. All rights reserved.
//

#include <iostream>
#include "common.h"
#include "hand.h"

void luatest()
{
    medlist_t *hl;
    card_array_t cards2;
    CardArray_Clear(&cards2);
    CardArray_PushBack(&cards2, 0x14);
    CardArray_PushBack(&cards2, 0x24);
    CardArray_PushBack(&cards2, 0x15);
    CardArray_PushBack(&cards2, 0x25);
    CardArray_PushBack(&cards2, 0x35);
    CardArray_PushBack(&cards2, 0x16);
    CardArray_PushBack(&cards2, 0x26);
    CardArray_PushBack(&cards2, 0x36);
    CardArray_PushBack(&cards2, 0x17);
    CardArray_PushBack(&cards2, 0x27);
    CardArray_PushBack(&cards2, 0x37);
    CardArray_PushBack(&cards2, 0x47);
    CardArray_PushBack(&cards2, 0x28);
    CardArray_PushBack(&cards2, 0x38);
    CardArray_PushBack(&cards2, 0x29);
    CardArray_PushBack(&cards2, 0x39);
    
    hl = HandList_AdvancedAnalyze(&cards2);
    
    HandList_Print(hl);
    
    HandList_Destroy(&hl);
}

int main(int argc, const char * argv[])
{
    luatest();
    
    return 0;
}

