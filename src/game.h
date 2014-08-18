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

#ifndef LANDLORD_GAME_H_
#define LANDLORD_GAME_H_

#include "player.h"
#include "deck.h"
#include "hand.h"
#include "mt19937.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GAME_PLAYERS    3
#define GAME_HAND_CARDS 17
#define GAME_REST_CARDS 3
#define GAME_BID_1      1
#define GAME_BID_2      2
#define GAME_BID_3      3


#define IncPlayerIdx(x)             (((x) + 1) % GAME_PLAYERS)
#define Game_GetCurrentPlayer(g)    (&(g)->players[(g)->playerIndex])
#define Game_IncPlayerIndex(g)      ((g)->playerIndex = IncPlayerIdx((g)->playerIndex))

typedef enum
{
    GameStatus_Halt = 0,
    GameStatus_Bid,
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

typedef struct game_s
{
    player_t        players[GAME_PLAYERS];      /* player array */
    deck_t          deck;                       /* deck */
    mt19937_t       mt;                         /* random context */
    hand_t          lastHand;                   /* last played hand */
    card_array_t    cardRecord;                 /* card record */
    card_array_t    kittyCards;                 /* kitty cards */
    int             bid;                        /* current bid */
    int             highestBidder;              /* for the highest bidder! */
    int             playerIndex;                /* current player index */
    int             landlord;                   /* landlord index */
    int             lastplay;                   /* who played the last hand */
    int             winner;                     /* who win the last game */
    int             status;                     /* game status */
    int             phase;                      /* game phase */
    
} game_t;

void Game_Init(game_t *game);

void Game_Clear(game_t *game);

void Game_Destroy(game_t *game);

void Game_Reset(game_t *game);

void Game_Play(game_t *game, uint32_t seed);

#ifdef __cplusplus
}
#endif

#endif /* LANDLORD_GAME_H_ */
