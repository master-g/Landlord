/*
 *  game.c
 *  Landlord
 *
 *  Created by Master.G on 14-2-5.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "game.h"

game_t *Game_Create(void)
{
    int i = 0;
    game_t *game = (game_t *)calloc(1, sizeof(game_t));
    for (i = 0; i < GAME_PLAYERS; i++)
    {
        game->players[i] = Player_CreateStandardAI();
        game->players[i]->identity = PlayerIdentity_Peasant;
    }
    
    game->deck = Deck_Create();
    game->mt = (mt19937_t *)calloc(1, sizeof(mt19937_t));
    Random_Init(game->mt, 0);
    
    return game;
}

void Game_Destroy(game_t *game)
{
    int i = 0;
    for (i = 0; i < GAME_PLAYERS; i++)
        Player_Destroy(game->players[i]);
    
    Deck_Destroy(game->deck);
    free(game->mt);
    free(game);
}

void Game_Reset(game_t *game)
{
    int i = 0;
    for (i = 0; i < GAME_PLAYERS; i++)
        Player_Clear(game->players[i]);
    
    Hand_Clear(&game->lastHand);
    game->playerIndex = 0;
    game->winner = 0;
    
    Deck_Reset(game->deck);
    
    CardArray_Clear(&game->cardRecord);
}

void Game_Play(game_t *game, uint32_t seed)
{
    int i = 0;
    int beat = 0;
    
    Random_Init(game->mt, seed);
    
    game->landlord = Random_int32(game->mt) % 3;
    game->players[game->landlord]->identity = PlayerIdentity_Landlord;
    
    Deck_Shuffle(game->deck, game->mt);
    
    /* test
    {
        card_array_t *arr1 = NULL;
        card_array_t *arr2 = NULL;
        card_array_t *arr3 = NULL;
        
        arr1 = CardArray_CreateFromString("♣2 ♦3 ♣3");
        arr2 = CardArray_CreateFromString("♦R ♥A ♣A ♥5 ♣5 ♠4 ♥4 ♣4");
        arr3 = CardArray_CreateFromString("♦3");
        
        CardArray_Copy(&game->players[0]->cards, arr1);
        CardArray_Copy(&game->players[1]->cards, arr2);
        CardArray_Copy(&game->players[2]->cards, arr3);
        
        CardArray_Destroy(arr1);
        CardArray_Destroy(arr2);
        CardArray_Destroy(arr3);
        
        game->landlord = 0;
    }
    */
    
    for (i = 0; i < GAME_PLAYERS; i++)
    {
        Deck_Deal(game->deck, &game->players[i]->cards, i == game->landlord ? GAME_HAND_CARDS + GAME_REST_CARDS : GAME_HAND_CARDS);
        Player_HandleEvent(Player_Event_GetReady, game->players[i], game);
    }
    
    game->status = GameStatus_Ready;
    game->playerIndex = game->landlord;
    game->phase = Phase_Play;
    
    while (game->status != GameStatus_Over)
    {
        if (game->phase == Phase_Play)
        {
            Player_HandleEvent(Player_Event_Play, Game_GetCurrentPlayer(game), game);
            game->phase = Phase_Query;
            
            CardArray_Concate(&game->cardRecord, &game->lastHand.cards);
#ifdef PRINT_GAME_LOG
            printf("\nPlayer ---- %d ---- played\n", game->playerIndex);
            Hand_Print(&game->lastHand);
#endif
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
#ifdef PRINT_GAME_LOG
                printf("\nPlayer ---- %d ---- passed\n", game->playerIndex);
#endif
            }
            else
            {
                game->phase = Phase_Query;
                CardArray_Concate(&game->cardRecord, &game->lastHand.cards);
#ifdef PRINT_GAME_LOG
                printf("\nPlayer ---- %d ---- beat\n", game->playerIndex);
                Hand_Print(&game->lastHand);
#endif
            }
        }
        
        Game_IncPlayerIndex(game);
        
        /* check if there is player win */
        for (i = 0; i < GAME_PLAYERS; i++)
        {
            if (game->players[i]->cards.length == 0)
            {
                game->status = GameStatus_Over;
                game->winner = i;
#ifdef PRINT_GAME_LOG
                printf("\nPlayer ++++ %d ++++ wins!\n", i);
#endif
                break;
            }
        }
    }
}
