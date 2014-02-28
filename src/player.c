/*
 *  player.c
 *  Landlord
 *
 *  Created by Master.G on 14-2-4.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "player.h"
#include "game.h"
#include "standard_ai.h"

player_t *Player_Create(void)
{
    player_t *player = (player_t *)calloc(1, sizeof(player_t));
    CardArray_Clear(&player->cards);
    CardArray_Clear(&player->record);
    
    return player;
}

player_t *Player_CreateStandardAI(void)
{
    player_t *player = Player_Create();
    player->eventHandlers[Player_Event_GetReady]    = StandardAI_GetReady;
    player->eventHandlers[Player_Event_Play]        = StandardAI_Play;
    player->eventHandlers[Player_Event_Beat]        = StandardAI_Beat;
    
    return player;
}

void Player_Destroy(player_t *player)
{
    HandList_Destroy(player->handlist);
    free(player);
}

void Player_Clear(player_t *player)
{
    HandList_Destroy(player->handlist);
    player->handlist = NULL;
    CardArray_Clear(&player->cards);
    CardArray_Clear(&player->record);
}

int Player_HandleEvent(int event, void *p, void *game)
{
    player_t *player = p;
    return player->eventHandlers[event](p, game);
}
