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

#ifndef LANDLORD_PLAYER_H_
#define LANDLORD_PLAYER_H_

#include "hand.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  Player_Event_GetReady = 0,
  Player_Event_Bid,
  Player_Event_Start,
  Player_Event_Play,
  Player_Event_Beat,

  Player_Event_Count

} PlayerEvent;

typedef enum {
  PlayerIdentity_Peasant = 0,
  PlayerIdentity_Landlord

} PlayerIdentity;

typedef enum {
  Player_Bid_Abstain = 0,
  Player_Bid_Bid

} PlayerBidAction;

typedef int (*PlayerEventHandler)(void *player, void *context);

typedef struct player_s {
  card_array_t cards;
  /* card array, will change during game play */
  card_array_t record;
  /* card record */
  medlist_t *handlist;
  /* the analyze result of cards */
  int identity;
  /* 0: peasant, 1: landlord */
  int seatId;
  /* 0, 1, 2 */
  int bid;
  /* 0, 1, 2, 3 */

  PlayerEventHandler eventHandlers[Player_Event_Count];

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
int Player_HandleEvent(void *player, int event, void *ctx);

/*
 * sort cards, analyze hands etc.
 *
 * int Player_GetReady(void *player, void *game);
 */

/*
 * decide whether to bid for landlord
 *
 * int Player_Bid(void *player, void *game);
 */

/*
 * the game will start, process landlord and kitty cards
 *
 * int Player_Start(void *player, void *game);
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

#ifdef __cplusplus
}
#endif

#endif /* LANDLORD_PLAYER_H_ */
