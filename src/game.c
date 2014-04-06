/*
 *  game.c
 *  Landlord
 *
 *  Created by Master.G on 14-2-5.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "game.h"

void Game_Init(game_t *game)
{
    int i = 0;
    for (i = 0; i < GAME_PLAYERS; i++)
    {
        Player_SetupStandardAI(&game->players[i]);
        game->players[i].identity = PlayerIdentity_Peasant;
        game->players[i].seatId = i;
        game->players[i].handlist = NULL;
    }

    game->bid = 0;
    game->playerIndex = 0;
    game->landlord = 0;
    game->lastplay = 0;
    game->winner = 0;
    game->status = 0;
    game->phase = 0;
    
    Deck_Reset(&game->deck);
    Random_Init(&game->mt, 0);
}

void Game_Clear(game_t *game)
{
    int i = 0;
    for (i = 0; i < GAME_PLAYERS; i++)
        Player_Clear(&game->players[i]);
}

void Game_Destroy(game_t *game)
{
    int i = 0;
    for (i = 0; i < GAME_PLAYERS; i++)
        Player_Clear(&game->players[i]);
    
    free(game);
}

void Game_Reset(game_t *game)
{
    int i = 0;
    for (i = 0; i < GAME_PLAYERS; i++)
    {
        Player_SetupStandardAI(&game->players[i]);
        Player_Clear(&game->players[i]);
    }
 
    game->bid = 0;
    game->playerIndex = 0;
    game->landlord = 0;
    game->lastplay = 0;
    game->winner = 0;
    game->status = 0;
    game->phase = 0;   

    Hand_Clear(&game->lastHand);
    Deck_Reset(&game->deck);
    CardArray_Clear(&game->cardRecord);
}

void Game_Play(game_t *game, uint32_t seed)
{
    int i = 0;
    int beat = 0;
    
    Random_Init(&game->mt, seed);
    
    game->landlord = Random_int32(&game->mt) % 3;
    game->players[game->landlord].identity = PlayerIdentity_Landlord;
    Player_SetupAdvancedAI(&game->players[game->landlord]);
    
    Deck_Shuffle(&game->deck, &game->mt);
    
    for (i = 0; i < GAME_PLAYERS; i++)
    {
        Deck_Deal(&game->deck, &game->players[i].cards, i == game->landlord ? GAME_HAND_CARDS + GAME_REST_CARDS : GAME_HAND_CARDS);
        Player_HandleEvent(Player_Event_GetReady, &game->players[i], game);
    }
    
    game->status = GameStatus_Ready;
    game->playerIndex = game->landlord;
    game->phase = Phase_Play;
    
    while (game->status != GameStatus_Over)
    {
        if (game->phase == Phase_Play)
        {
            Player_HandleEvent(Player_Event_Play, Game_GetCurrentPlayer(game), game);
            game->lastplay = game->playerIndex;
            game->phase = Phase_Query;
            
            CardArray_Concate(&game->cardRecord, &game->lastHand.cards);
            
            DBGLog("\nPlayer ---- %d ---- played\n", game->playerIndex);
            Hand_Print(&game->lastHand);
        }
        else if (game->phase == Phase_Query || game->phase == Phase_Pass)
        {
            beat = Player_HandleEvent(Player_Event_Beat, Game_GetCurrentPlayer(game), game);
            /* has beat in this phase */
            if (beat == 0)
            {
                /* two player pass */
                if (game->phase == Phase_Pass)
                    game->phase = Phase_Play;
                else
                    game->phase = Phase_Pass;
                
                DBGLog("\nPlayer ---- %d ---- passed\n", game->playerIndex);
            }
            else
            {
                game->lastplay = game->playerIndex;
                game->phase = Phase_Query;
                CardArray_Concate(&game->cardRecord, &game->lastHand.cards);
                
                DBGLog("\nPlayer ---- %d ---- beat\n", game->playerIndex);
                Hand_Print(&game->lastHand);
            }
        }
        
        Game_IncPlayerIndex(game);
        
        /* check if there is player win */
        for (i = 0; i < GAME_PLAYERS; i++)
        {
            if (game->players[i].cards.length == 0)
            {
                game->status = GameStatus_Over;
                game->winner = i;
                
                DBGLog("\nPlayer ++++ %d ++++ wins!\n", i);
                break;
            }
        }
    }
}
