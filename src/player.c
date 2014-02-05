/*
 *  player.c
 *  Landlord
 *
 *  Created by Master.G on 14-2-4.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "player.h"

player_t *Player_Create(void)
{
    player_t *player = (player_t *)calloc(1, sizeof(player_t));
    player->cards = CardArray_CreateEmpty();
    player->record = CardArray_CreateEmpty();
    
    return player;
}

void Player_Destroy(player_t *player)
{
    CardArray_Destroy(player->cards);
    CardArray_Destroy(player->record);
    HandList_Destroy(player->handlist);
    free(player);
}

void Player_Clear(player_t *player)
{
    CardArray_Clear(player->cards);
    CardArray_Clear(player->record);
}

void Player_GetReady(player_t *player)
{
    CardArray_Sort(player->cards, NULL);
    CardArray_Copy(player->record, player->cards);
    CardArray_Print(player->record);
}

hand_t *Player_Play(player_t *player)
{
    /*
     * TODO
     *
     */
    return NULL;
}

hand_t *Player_Beat(player_t *player, hand_t *tobeat)
{
    return NULL;
}
