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

#ifndef LANDLORD_DECK_H_
#define LANDLORD_DECK_H_

#include "card.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct deck_s {
  card_array_t cards;
  card_array_t used;

} deck_t;

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

#ifdef __cplusplus
}
#endif

#endif /* LANDLORD_DECK_H_ */
