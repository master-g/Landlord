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
    PlayerIdentity_Peasant = 0,
    PlayerIdentity_Landlord
    
} PlayerIdentity;

typedef struct player_t
{
    card_array_t    *cards;     /* card array, will change during game play */
    card_array_t    *record;    /* card record */
    hand_list_t     *handlist;  /* the analyze result of cards */
    int             identity;   /* 0: peasant, 1: landlord */
    
} player_t;

/*
 * create a player context
 */
player_t *Player_Create(void);

/*
 * destroy a player context
 */
void Player_Destroy(player_t *player);

/*
 * clear a player context
 */
void Player_Clear(player_t *player);

/*
 * sort cards, analyze hands etc.
 */
void Player_GetReady(player_t *player);

/*
 * free play
 */
void Player_Play(player_t *player, hand_t *hand);

/*
 * player must play a hand that can beat last player
 * if there is no hand can beat last player, return NULL
 */
void Player_Beat(player_t *player, hand_t *tobeat);

#endif /* LANDLORD_PLAYER_H_ */
