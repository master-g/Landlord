/*
 *  player.c
 *  Landlord
 *
 *  Created by Master.G on 14-2-4.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "player.h"

player_t *Player_Create(void)
{
    player_t *player = (player_t *)calloc(1, sizeof(player_t));
    player->cards = CardArray_CreateEmpty();
    player->record = CardArray_CreateEmpty();
    
    return player;
}

void Player_Destroy(player_t *player)
{
    CardArray_Destroy(player->cards);
    CardArray_Destroy(player->record);
    HandList_Destroy(player->handlist);
    free(player);
}

void Player_Clear(player_t *player)
{
    CardArray_Clear(player->cards);
    CardArray_Clear(player->record);
}

void Player_GetReady(player_t *player)
{
    CardArray_Sort(player->cards, NULL);
    CardArray_Copy(player->record, player->cards);
    CardArray_Print(player->record);
    player->handlist = HandList_StandardAnalyze(player->cards);
    /* HandList_Print(player->handlist); */
}

void Player_Play(player_t *player, player_t *others[], hand_t *hand)
{
    int countpair = 0;
    int countsolo = 0;
    int count = 0;
    int need = 0;
    int kicker = 0;
    hand_node_t *node = NULL;
    hand_node_t *temp = NULL;
    
    /* empty hands */
    if (player->handlist->first == NULL)
    {
        hand->type = 0;
        return;
    }
    
    /* last hand */
    if (player->handlist->first->next == NULL)
    {
        Hand_Copy(hand, player->handlist->first->hand);
        HandList_Remove(player->handlist, player->handlist->first);
        return;
    }
    
    /* try to find longest hand combination */
    node = HandList_Search(player->handlist, HAND_PRIMAL_TRIO | HAND_CHAIN);
    if (node != NULL)
    {
        /* copy hand*/
        Hand_Copy(hand, node->hand);
        
        /* how many kickers do we need */
        need = node->hand->cards->length / 3;
        
        /* remove hand node from hand list */
        HandList_Remove(player->handlist, node);
        
        /* count solo and pair number */
        temp = player->handlist->first;
        countpair = 0;
        countsolo = 0;
        while (temp->next != NULL)
        {
            if (temp->hand->type == (HAND_PRIMAL_PAIR))
                countpair++;
            else if (temp->hand->type == (HAND_PRIMAL_SOLO))
                countsolo++;
        }
        
        /* trio-pair-chain then trio-solo-chain */
        if (countsolo < need && countpair < need)
        {
            return;
        }
        else if (countpair >= countsolo)
        {
            kicker = HAND_KICKER_PAIR;
            count = countpair;
        }
        else
        {
            kicker = HAND_KICKER_SOLO;
            count = countsolo;
        }
        
        /* detach pairs from list */
        temp = player->handlist->first;
        while (count--)
        {
            if (temp->hand->type == kicker)
            {
                /* copy cards */
                CardArray_Concate(hand->cards, temp->hand->cards);
                HandList_Remove(player->handlist, temp);
                temp = player->handlist->first;
            }
            else
            {
                temp = temp->next;
            }
        }
        
        hand->type |= kicker;
        return;
    }
    
    node = HandList_Search(player->handlist, HAND_PRIMAL_PAIR | HAND_CHAIN);
    if (node != NULL)
    {
        Hand_Copy(hand, node->hand);
        HandList_Remove(player->handlist, node);
        return;
    }
    
    node = HandList_Search(player->handlist, HAND_PRIMAL_SOLO | HAND_CHAIN);
    if (node != NULL)
    {
        Hand_Copy(hand, node->hand);
        HandList_Remove(player->handlist, node);
        return;
    }
    
    /* just play */
    node = player->handlist->first;
    Hand_Copy(hand, node->hand);
    HandList_Remove(player->handlist, node);
}

int Player_Beat(player_t *player, player_t *others[], hand_t *tobeat)
{
    tobeat->type = 0;
    
    return 0;
}
