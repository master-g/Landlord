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
        game->players[i] = Player_Create();
    
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
    
    Deck_Reset(game->deck);
}

void Game_Play(game_t *game, uint32_t seed)
{
    int i = 0;
    int playeridx = 0;
    int beat = 0;
    player_t *others[2];
    hand_t hand;
    Hand_Clear(&hand);
    
    Random_Init(game->mt, seed);
    
    game->landlord = Random_int32(game->mt) % 3;
    game->players[game->landlord]->identity = PlayerIdentity_Landlord;
    
    Deck_Shuffle(game->deck, game->mt);
    
    /* test */
    {
        card_array_t *arr1 = NULL;
        card_array_t *arr2 = NULL;
        card_array_t *arr3 = NULL;
        
        hand_t hand;
        
        arr1 = CardArray_CreateFromString("♦J ♥7 ♠7 ♦4 ♥4 ♦4");
        arr2 = CardArray_CreateFromString("♥A ♥T ♦T ♦4 ♥4 ♦4");
        arr3 = CardArray_CreateFromString("♥A ♥A ♦5 ♠5 ♠7 ♦7 ♥6");
        
        Hand_Parse(&hand, arr1);
        Hand_Print(&hand);
        
        CardArray_Copy(game->players[0]->cards, arr1);
        CardArray_Copy(game->players[1]->cards, arr2);
        CardArray_Copy(game->players[2]->cards, arr3);
        
        CardArray_Destroy(arr1);
        CardArray_Destroy(arr2);
        CardArray_Destroy(arr3);
        
        game->landlord = 0;
    }
    
    for (i = 0; i < GAME_PLAYERS; i++)
    {
        /*
         Deck_Deal(game->deck, game->players[i]->cards, i == game->landlord ? GAME_HAND_CARDS + GAME_REST_CARDS : GAME_HAND_CARDS);
         */
        
        Player_GetReady(game->players[i]);
    }
    
    game->status = GameStatus_Ready;
    playeridx = game->landlord;
    game->phase = Phase_Play;
    
    while (game->status != GameStatus_Over)
    {
        others[0] = game->players[IncPlayerIdx(playeridx)];
        others[1] = game->players[IncPlayerIdx(playeridx+1)];
        
        switch (game->phase)
        {
            case Phase_Play:
            {
                Player_Play(game->players[playeridx], others, &hand);
                CardArray_Subtract(game->players[playeridx]->cards, &hand.cards);
                game->phase = Phase_Beat_1;
                
                printf("Player ---- %d ---- played\n", playeridx);
                Hand_Print(&hand);
                break;
            }
            case Phase_Beat_1:
            {
                beat = Player_Beat(game->players[playeridx], others, &hand);
                /* has beat in this phase */
                if (beat == 0)
                {
                    game->phase = Phase_Beat_2;
                }
                else
                {
                    printf("Player ---- %d ---- played\n", playeridx);
                    Hand_Print(&hand);
                }
                
                break;
            }
            case Phase_Beat_2:
            {
                beat = Player_Beat(game->players[playeridx], others, &hand);
                /* no beat */
                if (beat == 0)
                {
                    game->phase = Phase_Play;
                }
                else
                {
                    game->phase = Phase_Beat_1;
                    printf("Player ---- %d ---- played\n", playeridx);
                    Hand_Print(&hand);
                }
                
                break;
            }
            default:
                break;
        }
        
        playeridx = IncPlayerIdx(playeridx);
        
        /* check if there is player win */
        for (i = 0; i < GAME_PLAYERS; i++)
        {
            if (game->players[i]->cards->length == 0)
            {
                game->status = GameStatus_Over;
                break;
            }
        }
    }
}
