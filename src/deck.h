/*
 *  deck.h
 *  Landlord
 *
 *  Created by Master.G on 14-1-19.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#ifndef LANDLORD_DECK_H_
#define LANDLORD_DECK_H_

#include "card.h"

typedef struct deck_t
{
    card_array_t *cards;
    card_array_t *used;
    
} deck_t;

/*
 * create a new deck
 */
deck_t *Deck_Create(void);

/*
 * destroy a deck
 */
void Deck_Destroy(deck_t *deck);

/*
 * shuffle deck
 */
void Deck_Shuffle(deck_t *deck, void *mtctx);

/*
 * reset deck
 */
void Deck_Reset(deck_t *deck);

/*
 * deal single card
 */
uint8_t Deck_DealSingle(deck_t *deck);

/*
 * recyle single card
 */
void Deck_RecycleSingle(deck_t *deck, uint8_t card);

/*
 * deal cards
 */
int Deck_Deal(deck_t *deck, card_array_t *array, int count);

/*
 * recyle cards
 */
int Deck_Recycle(deck_t *deck, card_array_t *array);

/*
 * test deck
 */
void Deck_Test(void);

#endif /* LANDLORD_DECK_H_ */
