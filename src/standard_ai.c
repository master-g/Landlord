/*
 *  standard_ai.c
 *  Landlord
 *
 *  Created by Master.G on 14-2-28.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "standard_ai.h"
#include "game.h"

int StandardAI_GetReady(void *p, void *game)
{
    player_t *player = p;
    CardArray_Sort(&player->cards, NULL);
    CardArray_Copy(&player->record, &player->cards);
#ifdef PRINT_GAME_LOG
    CardArray_Print(&player->record);
#endif
    player->handlist = HandList_StandardAnalyze(&player->cards);
    
    return 0;
}

int StandardAI_Play(void *p, void *game)
{
    int countpair = 0;
    int countsolo = 0;
    int need = 0;
    int searchprimal = 0;
    int kicker = 0;
    player_t *player = p;
    hand_node_t *node = NULL;
    hand_node_t *temp = NULL;
    
    hand_t *hand = &((game_t *)game)->lastHand;
    
    do
    {
        /* empty hands */
        if (player->handlist->first == NULL)
        {
            hand->type = 0;
            break;
        }
        
        /* last hand */
        if (player->handlist->first->next == NULL)
        {
            Hand_Copy(hand, &player->handlist->first->hand);
            HandList_Remove(player->handlist, player->handlist->first);
            break;
        }
        
        /* try to find longest hand combination */
        node = HandList_Find(player->handlist, Hand_Format(HAND_PRIMAL_TRIO, HAND_KICKER_NONE, HAND_CHAIN));
        if (node != NULL)
        {
            /* copy hand*/
            Hand_Copy(hand, &node->hand);
            
            /* how many kickers do we need */
            need = node->hand.cards.length / 3;
            
            /* remove hand node from hand list */
            HandList_Remove(player->handlist, node);
            
            /* count solo and pair number */
            temp = player->handlist->first;
            countpair = 0;
            countsolo = 0;
            while (temp->next != NULL)
            {
                if (temp->hand.type == HAND_PRIMAL_PAIR)
                    countpair++;
                else if (temp->hand.type == HAND_PRIMAL_SOLO)
                    countsolo++;
                
                temp = temp->next;
            }
            
            /* trio-pair-chain then trio-solo-chain */
            if (countsolo < need && countpair < need)
            {
                break;
            }
            else if (countpair >= need)
            {
                searchprimal = HAND_PRIMAL_PAIR;
                kicker = HAND_KICKER_PAIR;
            }
            else
            {
                searchprimal = HAND_PRIMAL_SOLO;
                kicker = HAND_KICKER_SOLO;
            }
            
            /* detach pairs from list */
            temp = player->handlist->first;
            while (need > 0 && temp != NULL)
            {
                if (temp->hand.type == Hand_Format(searchprimal, HAND_KICKER_NONE, HAND_CHAIN_NONE))
                {
                    /* copy cards */
                    CardArray_Concate(&hand->cards, &temp->hand.cards);
                    HandList_Remove(player->handlist, temp);
                    temp = player->handlist->first;
                    need--;
                }
                else
                {
                    temp = temp->next;
                }
            }
            
            Hand_SetKicker(hand->type, kicker);
            break;
        }
        
        /* pair chain */
        node = HandList_Find(player->handlist, Hand_Format(HAND_PRIMAL_PAIR, HAND_KICKER_NONE, HAND_CHAIN));
        if (node != NULL)
        {
            Hand_Copy(hand, &node->hand);
            HandList_Remove(player->handlist, node);
            break;
        }
        
        /* solo chain */
        node = HandList_Find(player->handlist, Hand_Format(HAND_PRIMAL_SOLO, HAND_KICKER_NONE, HAND_CHAIN));
        if (node != NULL)
        {
            Hand_Copy(hand, &node->hand);
            HandList_Remove(player->handlist, node);
            break;
        }
        
        /* trio */
        node = HandList_Find(player->handlist, Hand_Format(HAND_PRIMAL_TRIO, HAND_KICKER_NONE, HAND_CHAIN_NONE));
        if (node != NULL && CARD_RANK(node->hand.cards.cards[0]) != CARD_RANK_2)
        {
            Hand_Copy(hand, &node->hand);
            HandList_Remove(player->handlist, node);
            
            /* pair */
            node = HandList_Find(player->handlist, Hand_Format(HAND_PRIMAL_PAIR, HAND_KICKER_NONE, HAND_CHAIN_NONE));
            if (node != NULL && CARD_RANK(node->hand.cards.cards[0]) != CARD_RANK_2)
            {
                kicker = HAND_KICKER_PAIR;
            }
            /* solo */
            else
            {
                node = HandList_Find(player->handlist, Hand_Format(HAND_PRIMAL_SOLO, HAND_KICKER_NONE, HAND_CHAIN_NONE));
                if (node != NULL && CARD_RANK(node->hand.cards.cards[0]) < CARD_RANK_2)
                    kicker = HAND_KICKER_SOLO;
            }
            
            if (node != NULL)
            {
                CardArray_Concate(&hand->cards, &node->hand.cards);
                Hand_SetKicker(hand->type, kicker);
                HandList_Remove(player->handlist, node);
                break;
            }
            
            /* no solo and pair, return with trio */
            break;
        }
        
        /* pair */
        node = HandList_Find(player->handlist, Hand_Format(HAND_PRIMAL_PAIR, HAND_KICKER_NONE, HAND_CHAIN_NONE));
        if (node != NULL && CARD_RANK(node->hand.cards.cards[0]) != CARD_RANK_2)
        {
            Hand_Copy(hand, &node->hand);
            HandList_Remove(player->handlist, node);
            break;
        }
        
        /* just play */
        node = player->handlist->first;
        Hand_Copy(hand, &node->hand);
        HandList_Remove(player->handlist, node);
        
    } while (0);
    
    CardArray_Subtract(&player->cards, &hand->cards);
    
    return 0;
}

int StandardAI_Beat(void *p, void *g)
{
    /*
     * HandList_SearchBeats can search for beat in loop mode
     * but we just simply find a beat here
     */
    int canbeat = 0;
    int i = 0;
    hand_t *tobeat;
    hand_t beat;
    player_t *player = p;
    player_t *prevplayer = NULL;
    player_t *teammate = NULL;
    player_t *landlord = NULL;
    game_t *game = g;
    Hand_Clear(&beat);
    
    tobeat = &((game_t *)game)->lastHand;
    
    canbeat = HandList_BestBeat(&player->cards, tobeat, &beat);
    /*
     canbeat = HandList_SearchBeat(&player->cards, tobeat, &beat);
     */
    
    /* peasant cooperation */
    /* 
     * TODO: teammate might not beat landlord
     * make sure that tobeat is played by teammate
     */
    prevplayer = game->players[(game->playerIndex + GAME_PLAYERS - 1) % GAME_PLAYERS];
    if (canbeat &&
        player->identity == PlayerIdentity_Peasant &&
        prevplayer->identity == PlayerIdentity_Peasant)
    {
        /* find teammate and landlord */
        for (i = 0; i < GAME_PLAYERS; i++)
        {
            if (game->players[i]->identity == PlayerIdentity_Landlord)
                landlord = game->players[i];
            
            if (game->players[i]->identity == PlayerIdentity_Peasant && game->players[i] != player)
                teammate = game->players[i];
        }
        
        /* don't bomb/nuke teammate */
        if (canbeat && (beat.type == Hand_Format(HAND_PRIMAL_BOMB, HAND_KICKER_NONE, HAND_CHAIN_NONE) ||
                        beat.type == Hand_Format(HAND_PRIMAL_NUKE, HAND_KICKER_NONE, HAND_CHAIN_NONE)))
            canbeat = 0;
        
        if (canbeat && teammate->cards.length < landlord->cards.length)
            canbeat = 0;
    }
    
    if (canbeat)
    {
        CardArray_Subtract(&player->cards, &beat.cards);
        HandList_Destroy(player->handlist);
        player->handlist = HandList_StandardAnalyze(&player->cards);
        Hand_Copy(tobeat, &beat);
    }
    
    return canbeat;
}
