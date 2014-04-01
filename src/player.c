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
#include "advanced_ai.h"

void Player_SetupStandardAI(player_t *player)
{
    player->eventHandlers[Player_Event_GetReady]    = StandardAI_GetReady;
    player->eventHandlers[Player_Event_Play]        = StandardAI_Play;
    player->eventHandlers[Player_Event_Beat]        = StandardAI_Beat;
}

void Player_SetupAdvancedAI(player_t *player)
{
    player->eventHandlers[Player_Event_GetReady]    = AdvancedAI_GetReady;
    player->eventHandlers[Player_Event_Play]        = StandardAI_Play;
    player->eventHandlers[Player_Event_Beat]        = AdvancedAI_Beat;
}

void Player_Destroy(player_t *player)
{
    HandList_Destroy(&player->handlist);
    free(player);
}

void Player_Clear(player_t *player)
{
    HandList_Destroy(&player->handlist);
    player->handlist = NULL;
    player->identity = PlayerIdentity_Peasant;
    CardArray_Clear(&player->cards);
    CardArray_Clear(&player->record);
}

int Player_HandleEvent(int event, void *p, void *game)
{
    player_t *player = (player_t *)p;
    return player->eventHandlers[event](p, game);
}
