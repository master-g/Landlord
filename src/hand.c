/*
 *  hand.c
 *  Landlord
 *
 *  Created by Master.G on 14-1-20.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "hand.h"

hand_t *Hand_Create(void)
{
    hand_t *hand = (hand_t *)calloc(1, sizeof(hand_t));
    hand->cards = CardArray_CreateEmpty();
    
    return hand;
}


void Hand_Destroy(hand_t *hand)
{
    CardArray_Destroy(hand->cards);
    free(hand);
}

void Hand_Clear(hand_t *hand)
{
    CardArray_Clear(hand->cards);
    hand->type = 0;
}

int Hand_Validate(hand_t *hand)
{
    return 0;
}

/*
 * ************************************************************
 * parser
 * ************************************************************
 */

void _Hand_Count_Rank(card_array_t *array, int *count)
{
    int i = 0;
    memset(count, 0, sizeof(int) * CARD_RANK_END);
    for (i = 0; i < array->length; i++)
        count[CARD_RANK(array->cards[i])]++;
}

void _Hand_Parse_1(hand_t *hand, card_array_t *array)
{
    /* solo */
    CardArray_Copy(hand->cards, array);
    hand->type = HAND_PRIMAL_SOLO | HAND_KICKER_NONE | HAND_CHAIN_NONE;
}

void _Hand_Parse_2(hand_t *hand, card_array_t *array)
{
    /* pair or nuke */
    if (array->cards[0] == array->cards[1])
    {
        hand->type = CARD_RANK(array->cards[0]) == CARD_RANK_R ? HAND_PRIMAL_NUKE : HAND_PRIMAL_PAIR;
        CardArray_Copy(hand->cards, array);
    }
}

void _Hand_Parse_3(hand_t *hand, card_array_t *array)
{
    /* trio */
    if (CARD_RANK(array->cards[0]) ==
        CARD_RANK(array->cards[1]) ==
        CARD_RANK(array->cards[2]))
    {
        CardArray_Copy(hand->cards, array);
        hand->type = HAND_PRIMAL_TRIO;
    }
}

void _Hand_Parse_4(hand_t *hand, card_array_t *array)
{
    int count[CARD_RANK_END];
    int i = 0;
    int trio = 0;
    uint8_t kicker = 0;
    
    /* trio solo | bomb */
    if (CARD_RANK(array->cards[0]) ==
        CARD_RANK(array->cards[1]) ==
        CARD_RANK(array->cards[2]) ==
        CARD_RANK(array->cards[3]))
    {
        CardArray_Copy(hand->cards, array);
        hand->type = HAND_PRIMAL_BOMB;
    }
    else
    {
        _Hand_Count_Rank(array, count);
        for (i = CARD_RANK_BEG; i < CARD_RANK_END; i++)
        {
            if (count[i] == 3)
            {
                /* trio found */
                trio = i;
                break;
            }
        }
        
        if (trio != 0)
        {
            /* format trio into AAAB style */
            for (i = 0; i < array->length; i++)
            {
                if (CARD_RANK(array->cards[i]) == trio)
                    CardArray_PushBack(hand->cards, array->cards[i]);
                else
                    kicker = array->cards[i];
            }
            
            CardArray_PushBack(hand->cards, kicker);
            hand->type = HAND_PRIMAL_TRIO | HAND_KICKER_SOLO | HAND_CHAIN_NONE;
        }
    }
}

void _Hand_Parse_5(hand_t *hand, card_array_t *array)
{
}

void _Hand_Parse_Default(hand_t *hand, card_array_t *array)
{
    hand->type = 0;
}

typedef void (* Hand_Parser)(hand_t *, card_array_t *);

void Hand_Parse(hand_t *hand, card_array_t *array)
{
    Hand_Parser parser[HAND_MAX_LENGTH + 1];
    
    if (array->length < HAND_MIN_LENGTH || array->length > HAND_MAX_LENGTH)
    {
        hand->type = HAND_PRIMAL_NONE;
    }
    else
    {
        parser[array->length](hand, array);
    }
}

/*
 * ************************************************************
 * comparators
 * ************************************************************
 */

/* one of a, b must be bomb or nuke */
int _Hand_Compare_Bomb(hand_t *a, hand_t *b)
{
    int ret = HAND_CMP_ILLEGAL;
    
    /* same type same cards, equal */
    if ((a->type == b->type) && (a->cards->cards[0] == b->cards->cards[0]))
        ret = HAND_CMP_EQUAL;
    /* both are bombs, compare by card rank */
    else if (a->type == HAND_PRIMAL_BOMB && b->type == HAND_PRIMAL_BOMB)
        ret = CARD_RANK(a->cards->cards[0]) > CARD_RANK(b->cards->cards[0]) ? HAND_CMP_GREATER : HAND_CMP_LESS;
    else
        ret = Hand_GetPrimal(a->type) > Hand_GetPrimal(b->type) ? HAND_CMP_GREATER : HAND_CMP_LESS;
    
    return ret;
}

int Hand_Compare(hand_t *a, hand_t *b)
{
    int result = HAND_CMP_ILLEGAL;
    
    /* 
     * different hand type
     * check for bomb and nuke
     */
    if (a->type != b->type)
    {
        if (a->type != HAND_PRIMAL_NUKE &&
            a->type != HAND_PRIMAL_BOMB &&
            b->type != HAND_PRIMAL_NUKE &&
            b->type != HAND_PRIMAL_BOMB)
        {
            result = HAND_CMP_ILLEGAL;
        }
        else
        {
            result = _Hand_Compare_Bomb(a, b);
        }
    }
    else /* same hand type and with no bombs */
    {
        /* same hand type but different length */
        if (a->cards->length != b->cards->length)
        {
            result = HAND_CMP_ILLEGAL;
        }
        else /* same hand type and same length */
        {
            if (CARD_RANK(a->cards->cards[0]) == CARD_RANK(b->cards->cards[0]))
                result = HAND_CMP_EQUAL;
            else
                result = CARD_RANK(a->cards->cards[0]) > CARD_RANK(b->cards->cards[0]) ? HAND_CMP_GREATER : HAND_CMP_LESS;
        }
    }
    
    return result;
}

/*
 * ************************************************************
 * hand graph
 * ************************************************************
 */

void LinkList_Destroy(link_list_t *links)
{
    link_list_t *cur;
    link_list_t *tmp;
    
    for (cur = links; cur != NULL; )
    {
        tmp = cur;
        cur = cur->next;
        
        free(tmp);
    }
}

void Graph_Destroy(graph_t *graph)
{
    graph_t *cur;
    graph_t *tmp;
    
    for (cur = graph; cur != NULL; )
    {
        tmp = cur;
        cur = cur->next;
        
        LinkList_Destroy(tmp->links);
        free(tmp);
    }
}

graph_t *Graph_Parse(card_array_t *array)
{
    return NULL;
}