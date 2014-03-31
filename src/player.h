/*
 *  player.h
 *  Landlord
 *
 *  Created by Master.G on 14-2-4.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#ifndef LANDLORD_PLAYER_H_
#define LANDLORD_PLAYER_H_

#include "hand.h"

typedef enum
{
    Player_Event_GetReady = 0,
    Player_Event_Play,
    Player_Event_Beat,
    
    Player_Event_Count
    
} PlayerEvent;

typedef enum
{
    PlayerIdentity_Peasant = 0,
    PlayerIdentity_Landlord
    
} PlayerIdentity;

typedef int (*PlayerEventHandler)(void *player, void *context);

typedef struct player_s
{
    card_array_t    cards;      /* card array, will change during game play */
    card_array_t    record;     /* card record */
    medlist_t       *handlist;  /* the analyze result of cards */
    int             identity;   /* 0: peasant, 1: landlord */
    int             seatId;     /* 0, 1, 2 */
    
    PlayerEventHandler  eventHandlers[Player_Event_Count];
    
} player_t;

/*
 * setup standard AI player
 */
void Player_SetupStandardAI(player_t *player);

/*
 * setup advanced AI player
 */
void Player_SetupAdvancedAI(player_t *player);

/*
 * destroy a player context
 */
void Player_Destroy(player_t *player);

/*
 * clear a player context
 */
void Player_Clear(player_t *player);

/* 
 * handle event 
 */
int Player_HandleEvent(int event, void *player, void *game);

/*
 * sort cards, analyze hands etc.
 *
 * int Player_GetReady(void *player, void *game);
 */

/*
 * free play, result will return by hand_t *
 * int Player_Play(void *player, void *game);
 */

/*
 * player must play a hand that can beat last player
 * if there is no hand can beat last player, tobeat->type will be 0
 *
 * int Player_Beat(void *player, void *game);
 */

#endif /* LANDLORD_PLAYER_H_ */
