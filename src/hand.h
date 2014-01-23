/*
 *  hand.h
 *  Landlord
 *
 *  Created by Master.G on 14-1-20.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#ifndef LANDLORD_HAND_H_
#define LANDLORD_HAND_H_

#include "card.h"

/* primal hands */
#define HAND_PRIMAL_NONE        0x00
#define HAND_PRIMAL_NUKE        0x06
#define HAND_PRIMAL_BOMB        0x05
#define HAND_PRIMAL_FOUR        0x04
#define HAND_PRIMAL_TRIO        0x03
#define HAND_PRIMAL_PAIR        0x02
#define HAND_PRIMAL_SOLO        0x01

/* kicker hands */
#define HAND_KICKER_NONE        0x00
#define HAND_KICKER_SOLO        0x10
#define HAND_KICKER_PAIR        0x20
#define HAND_KICKER_DUAL_SOLO   0x30
#define HAND_KICKER_DUAL_PAIR   0x40

/* chain */
#define HAND_CHAIN_NONE         0x00
#define HAND_CHAIN              0x80

#define Hand_GetPrimal(h)       ((h)&0x0F)
#define Hand_GetKicker(h)       ((h)&0x70)
#define Hand_GetChain(h)        ((h)&0x80)

typedef enum
{
    HAND_CMP_ILLEGAL = -3,
    HAND_CMP_LESS    = -1,
    HAND_CMP_EQUAL   = 0,
    HAND_CMP_GREATER = 1
    
}HandCompareResult;

/*
 * hand is a valid card set that can play.
 * most of the time Hand_XXX won't check
 */
typedef struct hand_t
{
    uint8_t         type;
    card_array_t    *cards;
    
}hand_t;

/* 
 * create an empty hand
 */
hand_t *Hand_Create(void);

/*
 * destroy a hand
 */
void Hand_Destroy(hand_t *hand);

/* 
 * clear a hand 
 */
void Hand_Clear(hand_t *hand);

/*
 * validate a hand
 */
int Hand_Validate(hand_t *hand);

/*
 * parse an card array to hand
 */
void Hand_Parse(hand_t *hand, card_array_t *array);

/*
 * compare two hands
 */
int Hand_Compare(hand_t *a, hand_t *b);


#endif
