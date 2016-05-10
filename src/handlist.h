/*
The MIT License (MIT)

Copyright (c) 2014 Master.G

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef LANDLORD_HANDLIST_H
#define LANDLORD_HANDLIST_H

#include "hand.h"
#include "ruiko_algorithm.h"

/* ************************************************************
 * hand list
 * ************************************************************/

typedef int (*HandList_EvaluateFunc)(card_array_t *);

/*
 * push a hand into the front of hand list
 */
void HandList_PushFront(rk_list_t *hl, hand_t *hand);

/*
 * remove a hand from hand list
 */
void HandList_Remove(rk_list_t *hl, rk_list_node_t *node);

/*
 * search a specific hand type from hand list
 */
rk_list_node_t *HandList_Find(rk_list_t *hl, int handtype);

/*
 * get payload as hand_t
 */
#define HandList_GetHand(h) ((hand_t *)((h)->payload))

/* ************************************************************
 * utils
 * ************************************************************/

/*
 * search a beat in card array
 */
int HandList_SearchBeat(card_array_t *cards, hand_t *tobeat, hand_t *beat);

/*
 * search all the beats
 */
rk_list_t *HandList_SearchBeatList(card_array_t *cards, hand_t *tobeat);

/*
 * standard analyze a card array into hand list
 */
rk_list_t *HandList_StandardAnalyze(card_array_t *array);

/*
 * count how many primal hands in array
 */
int HandList_StandardEvaluator(card_array_t *array);

/*
 * advanced hand analyze
 */
rk_list_t *HandList_AdvancedAnalyze(card_array_t *array);

/*
 * advanced hand evaluator
 */
int HandList_AdvancedEvaluator(card_array_t *array);

/*
 * search best beat from given cards
 */
int HandList_BestBeat(card_array_t *array, hand_t *tobeat, hand_t *beat, HandList_EvaluateFunc func);

/*
 * print hand_list_t
 */
void HandList_Print(rk_list_t *hl);

#endif /* LANDLORD_HANDLIST_H */
