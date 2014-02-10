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

#define IncPlayerIdx(x)     (((x) + 1) % GAME_PLAYERS)
#define OtherPlayerIdx1(x)  IncPlayerIdx(x)
#define OtherPlayerIdx2(x)  IncPlayerIdx(x+1)

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
    Phase_Beat_1,
    Phase_Beat_2
    
} StagePhase;

typedef struct game_t
{
    player_t    *players[GAME_PLAYERS];     /* player array */
    deck_t      *deck;                      /* deck */
    mt19937_t   *mt;                        /* random context */
    int         landlord;                   /* landlord index */
    int         status;                     /* game status */
    int         phase;                      /* game phase */
    
} game_t;

game_t *Game_Create(void);

void Game_Destroy(game_t *game);

void Game_Reset(game_t *game);

void Game_Play(game_t *game, uint32_t seed);


#endif /* LANDLORD_GAME_H_ */
