/*
 *  game.h
 *  Landlord
 *
 *  Created by Master.G on 14-2-5.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#ifndef LANDLORD_GAME_H_
#define LANDLORD_GAME_H_

#include "player.h"
#include "deck.h"
#include "hand.h"
#include "mt19937.h"

#define GAME_PLAYERS    3
#define GAME_HAND_CARDS 17
#define GAME_REST_CARDS 3

#define IncPlayerIdx(x)             (((x) + 1) % GAME_PLAYERS)
#define Game_GetCurrentPlayer(g)    (&(g)->players[(g)->playerIndex])
#define Game_IncPlayerIndex(g)      ((g)->playerIndex = IncPlayerIdx((g)->playerIndex))

typedef enum
{
    GameStatus_Halt = 0,
    GameStatus_Ready,
    GameStatus_Pause,
    GameStatus_Over
    
} GameStatus;

typedef enum
{
    Phase_Play = 0,
    Phase_Query,
    Phase_Pass
    
} StagePhase;

typedef struct game_t
{
    player_t        players[GAME_PLAYERS];      /* player array */
    deck_t          deck;                       /* deck */
    mt19937_t       mt;                         /* random context */
    hand_t          lastHand;                   /* last played hand */
    card_array_t    cardRecord;                 /* card record */
    int             playerIndex;                /* current player index */
    int             landlord;                   /* landlord index */
    int             lastplay;                   /* who played last hand */
    int             winner;                     /* winner index */
    int             status;                     /* game status */
    int             phase;                      /* game phase */
    
} game_t;

void Game_Init(game_t *game);

void Game_Clear(game_t *game);

void Game_Destroy(game_t *game);

void Game_Reset(game_t *game);

void Game_Play(game_t *game, uint32_t seed);


#endif /* LANDLORD_GAME_H_ */
