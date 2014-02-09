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

void Player_Play(player_t *player, hand_t *hand)
{
    /*
     * TODO
     * search longest hand
     */
    hand_list_t *hl = NULL;
    
    if (player->handlist == NULL)
        player->handlist = HandList_StandardAnalyze(player->cards);
    
    hl = player->handlist;
    player->handlist = hl->next;
    
    CardArray_Subtract(player->cards, hl->hand->cards);
    
    hand = hl->hand;
    hl->next = NULL;
    hl->hand = NULL;
    HandList_Destroy(hl);
    
    /*
    for (hl = player->handlist; hl != NULL; hl = hl->next)
    {
        Hand_Print(hl->hand);
    }
     */
}

void Player_Beat(player_t *player, hand_t *tobeat)
{
    tobeat->type = 0;
}
