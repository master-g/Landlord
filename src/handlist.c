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

/*
 * TODO
 * add handlist_sort
 */

#include "handlist.h"
#include "lmath.h"

#define BEAT_NODE_CAPACITY          255

/*
 * ************************************************************
 * hand list
 * ************************************************************
 */
void HandList_PushFront(rk_list_t *hl, hand_t *hand) {
  hand_t *payload = (hand_t *) malloc(sizeof(hand_t));
  Hand_Copy(payload, hand);
  rk_list_push(hl, payload);
}

void HandList_Remove(rk_list_t *hl, hand_t *hand) {
  void *payload = NULL;
  rk_list_foreach(hl, first, next, cur) {
    if (cur->payload == hand) {
      payload = rk_list_remove(hl, cur);
      break;
    }
  }

  free(payload);
}

int _HandList_FindFunc(void *payload, void *context) {
  return ((hand_t *) payload)->type == *(int *) context ? 1 : 0;
}

hand_t *HandList_Find(rk_list_t *hl, int handtype) {
  return rk_list_search(hl, &handtype, _HandList_FindFunc);
}

/*
 * ************************************************************
 * beat search
 * ************************************************************
 */

/* beat search context */
typedef struct hand_ctx_s {
  /* rank count */
  int count[CARD_RANK_END];
  /* original cards */
  card_array_t cards;
  /* reverse sorted cards */
  card_array_t rcards;

} hand_ctx_t;

#define HandCtx_Clear(ctx) memset((ctx), 0, sizeof(hand_ctx_t))

void HandCtx_Setup(hand_ctx_t *ctx, card_array_t *array) {
  /* setup search context */
  HandCtx_Clear(ctx);

  Hand_CountRank(array, ctx->count, NULL);
  CardArray_Copy(&ctx->cards, array);
  CardArray_Copy(&ctx->rcards, array);
  CardArray_Sort(&ctx->cards, NULL);
  CardArray_Reverse(&ctx->rcards);
}

int _HandList_SearchBeat_Primal(hand_ctx_t *ctx,
                                hand_t *tobeat,
                                hand_t *beat,
                                int primal) {
  int i = 0;
  int canbeat = 0;
  int *count = NULL;
  int rank = 0;
  card_array_t *temp = NULL;
  int tobeattype = tobeat->type;

  count = ctx->count;
  temp = &ctx->rcards;

  rank = CARD_RANK(tobeat->cards.cards[0]);

  /* search for primal */
  for (i = 0; i < temp->length; i++) {
    if ((CARD_RANK(temp->cards[i]) > rank) &&
      (count[CARD_RANK(temp->cards[i])] >= primal)) {
      Hand_Clear(beat);
      beat->type = (uint8_t) tobeattype;
      CardArray_PushBackCards(&beat->cards, temp, i, primal);
      canbeat = 1;
      break;
    }
  }

  return canbeat;
}

int _HandList_SearchBeat_Bomb(hand_ctx_t *ctx, hand_t *tobeat, hand_t *beat) {
  int canbeat = 0;
  int *count = NULL;
  int i = 0;
  card_array_t *cards = &ctx->cards;

  count = ctx->count;

  /* can't beat nuke */
  if (tobeat->type ==
    Hand_Format(HAND_PRIMAL_NUKE, HAND_KICKER_NONE, HAND_CHAINLESS))
    return 0;

  /* search for a higher rank bomb */
  if (tobeat->type == HAND_PRIMAL_BOMB) {
    canbeat = _HandList_SearchBeat_Primal(ctx, tobeat, beat, 4);
  } else {
    /* tobeat is not a nuke or bomb, search a bomb to beat it */
    for (i = 0; i < ctx->cards.length; i++) {
      if (count[CARD_RANK(ctx->cards.cards[i])] == 4) {
        canbeat = 1;
        Hand_Clear(beat);
        CardArray_CopyRank(&beat->cards, cards, CARD_RANK(ctx->cards.cards[i]));
        break;
      }
    }
  }

  /* search for nuke */
  if (canbeat == 0) {
    if (count[CARD_RANK_r] && count[CARD_RANK_R]) {
      canbeat = 1;
      Hand_Clear(beat);
      beat->type =
        Hand_Format(HAND_PRIMAL_NUKE, HAND_KICKER_NONE, HAND_CHAINLESS);
      CardArray_CopyRank(&beat->cards, cards, CARD_RANK_R);
      CardArray_CopyRank(&beat->cards, cards, CARD_RANK_r);
    }
  } else {
    beat->type =
      Hand_Format(HAND_PRIMAL_BOMB, HAND_KICKER_NONE, HAND_CHAINLESS);
  }

  return canbeat;
}

/*
 * for a standard 54 card set, each rank has four cards
 * so it is impossible for two trio of the same rank at the same time
 *
 * a) player_1 SEARCH_BEAT player_2 : impossible for 333 vs 333
 *
 * BUT
 *
 * b) player_1 SEARCH_BEAT_LOOP player_1_prev_beat : possible for 333 vs 333
 *
 */
int _HandList_SearchBeat_TrioKicker(hand_ctx_t *ctx,
                                    hand_t *tobeat,
                                    hand_t *beat,
                                    int kick) {
  int i = 0;
  int canbeat = 0;
  int cantriobeat = 0;
  int tobeattype = tobeat->type;
  int *count = NULL;
  card_array_t temp;
  hand_t htrio, hkick, htriobeat, hkickbeat;

  Hand_Clear(&htrio);
  Hand_Clear(&hkick);
  Hand_Clear(&htriobeat);
  Hand_Clear(&hkickbeat);

  count = ctx->count;
  CardArray_Copy(&temp, &ctx->rcards);

  /* copy hands */
  CardArray_PushBackCards(&htrio.cards, &tobeat->cards, 0, 3);
  CardArray_PushBackCards(&hkick.cards, &tobeat->cards, 3, kick);

  /* same rank trio , case b */
  if (CardArray_IsContain(&temp, &htrio.cards)) {
    /* keep trio beat */
    CardArray_Copy(&htriobeat.cards, &htrio.cards);
    CardArray_RemoveRank(&temp, CARD_RANK(htriobeat.cards.cards[0]));

    /* search for a higher kicker */
    /* round 1: only search those count[rank] == kick */
    for (i = 0; i < temp.length; i++) {
      if ((count[CARD_RANK(temp.cards[i])] >= kick) &&
        (CARD_RANK(temp.cards[i]) > CARD_RANK(hkick.cards.cards[0]))) {
        CardArray_Clear(&hkickbeat.cards);
        CardArray_PushBackCards(&hkickbeat.cards, &temp, i, kick);
        canbeat = 1;
        break;
      }
    }

    /* if kicker can't beat, restore trio */
    if (canbeat == 0) {
      CardArray_Clear(&htriobeat.cards);
      CardArray_Copy(&temp, &ctx->rcards);
    }
  }

  /*
   * same rank trio not found
   * OR
   * same rank trio found, but kicker can't beat
   */
  if (canbeat == 0) {
    cantriobeat = _HandList_SearchBeat_Primal(ctx,
                                              &htrio,
                                              &htriobeat,
                                              HAND_PRIMAL_TRIO);

    /* trio beat found, search for kicker beat */
    if (cantriobeat == 1) {
      /* remove trio from temp */
      CardArray_RemoveRank(&temp, CARD_RANK(htriobeat.cards.cards[0]));

      /* search for a kicker */
      for (i = 0; i < temp.length; i++) {
        if (count[CARD_RANK(temp.cards[i])] >= kick) {
          CardArray_PushBackCards(&hkickbeat.cards, &temp, i, kick);
          canbeat = 1;
          break;
        }
      }
    }
  }

  /* beat */
  if (canbeat == 1) {
    Hand_Clear(beat);
    CardArray_Concat(&beat->cards, &htriobeat.cards);
    CardArray_Concat(&beat->cards, &hkickbeat.cards);
    beat->type = (uint8_t) tobeattype;
  }

  return canbeat;
}

int _HandList_SearchBeat_Chain(hand_ctx_t *ctx,
                               hand_t *tobeat,
                               hand_t *beat,
                               int duplicate) {
  int canbeat = 0;
  int found = 0;
  int i, j, k, chainlength;
  int tobeattype = tobeat->type;
  uint8_t footer = 0;
  int *count = NULL;
  card_array_t *cards = &ctx->cards;
  card_array_t temp;

  count = ctx->count;
  CardArray_Clear(&temp);

  chainlength = tobeat->cards.length / duplicate;
  footer = CARD_RANK(tobeat->cards.cards[tobeat->cards.length - 1]);

  /* search for beat chain in rank counts */
  for (i = footer + 1; i <= CARD_RANK_2 - chainlength; i++) {
    found = 1;

    for (j = 0; j < chainlength; j++) {
      /* check if chain breaks */
      if (count[i + j] < duplicate) {
        found = 0;
        break;
      }
    }

    if (found) {
      footer = i;         /* beat footer rank */
      k = duplicate;      /* how many cards needed for each rank */

      for (i = cards->length; i >= 0 && chainlength > 0; i--) {
        if (CARD_RANK(cards->cards[i]) == footer) {
          CardArray_PushFront(&temp, cards->cards[i]);
          k--;

          if (k == 0) {
            k = duplicate;
            chainlength--;
            footer++;
          }
        }
      }

      break;
    }
  }

  if (found) {
    beat->type = (uint8_t) tobeattype;
    CardArray_Copy(&beat->cards, &temp);
    canbeat = 1;
  }

  return canbeat;
}

int _HandList_SearchBeat_TrioKickerChain(hand_ctx_t *ctx,
                                         hand_t *tobeat,
                                         hand_t *beat,
                                         int kc) {
  int canbeat = 0;
  int cantriobeat = 0;
  int i, j, chainlength;
  int tobeattype = tobeat->type;
  int count[CARD_RANK_END];
  int kickcount[CARD_RANK_END];
  int combrankmap[CARD_RANK_END];
  int rankcombmap[CARD_RANK_END];
  int comb[CARD_RANK_END];
  card_array_t temp;
  hand_t htrio, hkick, htriobeat, hkickbeat;

  /* setup variables */
  memcpy(count, ctx->count, sizeof(int) * CARD_RANK_END);

  Hand_Clear(&htrio);
  Hand_Clear(&hkick);
  Hand_Clear(&htriobeat);
  Hand_Clear(&hkickbeat);

  CardArray_Copy(&temp, &ctx->rcards);
  chainlength = tobeat->cards.length / (HAND_PRIMAL_TRIO + kc);

  /* copy tobeat cards */
  CardArray_PushBackCards(&htrio.cards,
                          &tobeat->cards,
                          0,
                          3 * chainlength);
  CardArray_PushBackCards(&hkick.cards,
                          &tobeat->cards,
                          3 * chainlength,
                          chainlength * kc);

  htrio.type = Hand_Format(HAND_PRIMAL_TRIO, HAND_KICKER_NONE, HAND_CHAIN);

  /* self beat, see _HandList_SearchBeat_TrioKicker */
  if (CardArray_IsContain(&temp, &htrio.cards)) {
    int n = 0; /* combination total */

    /* remove trio from kickcount */
    memcpy(kickcount, count, sizeof(int) * CARD_RANK_END);

    for (i = 0; i < htrio.cards.length; i += 3)
      kickcount[CARD_RANK(htrio.cards.cards[i])] = 0;

    /* remove count < kc and calculate n */
    for (i = CARD_RANK_3; i < CARD_RANK_END; i++) {
      if (kickcount[i] < kc)
        kickcount[i] = 0;
      else
        n++;
    }

    /* setup comb-rank and rank-comb map */
    j = 0;
    memset(combrankmap, -1, sizeof(int) * CARD_RANK_END);
    memset(rankcombmap, -1, sizeof(int) * CARD_RANK_END);

    for (i = CARD_RANK_3; i < CARD_RANK_END; i++) {
      if (kickcount[i] != 0) {
        combrankmap[j] = i;
        rankcombmap[i] = j;
        j++;
      }
    }

    /* setup combination */
    j = 0;
    memset(comb, -1, sizeof(int) * CARD_RANK_END);

    for (i = 0; i < hkick.cards.length; i += kc)
      comb[j++] = rankcombmap[CARD_RANK(hkick.cards.cards[i])];

    /* find next combination */
    if (LMath_NextComb(comb, chainlength, n)) {
      /* next combination found, copy kickers */
      for (i = 0; i < chainlength; i++) {
        int rank = combrankmap[comb[i]];

        for (j = 0; j < temp.length; j++) {
          if (CARD_RANK(temp.cards[j]) == rank) {
            CardArray_PushBackCards(&hkickbeat.cards, &temp, j, kc);
            break;
          }
        }
      }

      canbeat = 1;

      /* copy trio to beat */
      CardArray_Concat(&htriobeat.cards, &htrio.cards);
      CardArray_Sort(&hkickbeat.cards, NULL);
    }
  }

  /* can't find same rank trio chain, search for higher rank trio */
  if (canbeat == 0) {
    /* restore rank count */
    memcpy(count, ctx->count, sizeof(int) * CARD_RANK_END);

    cantriobeat = _HandList_SearchBeat_Chain(ctx, &htrio, &htriobeat, 3);

    /* higher rank trio chain found, search for kickers */
    if (cantriobeat) {
      /* remove trio from temp */
      for (i = 0; i < htriobeat.cards.length; i += 3) {
        CardArray_RemoveRank(&temp, CARD_RANK(htriobeat.cards.cards[i]));
        count[CARD_RANK(htriobeat.cards.cards[0])] = 0;
      }

      for (j = 0; j < chainlength; j++) {
        for (i = 0; i < temp.length; i++) {
          if (count[CARD_RANK(temp.cards[i])] >= kc) {
            CardArray_PushBackCards(&hkickbeat.cards, &temp, i, kc);
            CardArray_RemoveRank(&temp, CARD_RANK(temp.cards[i]));
            break;
          }
        }
      }

      if (hkickbeat.cards.length == kc * chainlength) canbeat = 1;
    }
  }

  /* final */
  if (canbeat) {
    Hand_Clear(beat);
    CardArray_Concat(&beat->cards, &htriobeat.cards);
    CardArray_Concat(&beat->cards, &hkickbeat.cards);
    beat->type = (uint8_t) tobeattype;
  }

  return canbeat;
}

int _HandList_SearchBeat(card_array_t *cards, hand_t *tobeat, hand_t *beat) {
  int canbeat = 0;
  hand_ctx_t ctx;

  /* setup search context */
  HandCtx_Setup(&ctx, cards);

  /* start search */
  switch (tobeat->type) {
    case Hand_Format(HAND_PRIMAL_SOLO, HAND_KICKER_NONE, HAND_CHAINLESS):
      canbeat =
        _HandList_SearchBeat_Primal(&ctx, tobeat, beat, HAND_PRIMAL_SOLO);
      break;

    case Hand_Format(HAND_PRIMAL_PAIR, HAND_KICKER_NONE, HAND_CHAINLESS):
      canbeat =
        _HandList_SearchBeat_Primal(&ctx, tobeat, beat, HAND_PRIMAL_PAIR);
      break;

    case Hand_Format(HAND_PRIMAL_TRIO, HAND_KICKER_NONE, HAND_CHAINLESS):
      canbeat =
        _HandList_SearchBeat_Primal(&ctx, tobeat, beat, HAND_PRIMAL_TRIO);
      break;

    case Hand_Format(HAND_PRIMAL_TRIO, HAND_KICKER_PAIR, HAND_CHAINLESS):
      canbeat =
        _HandList_SearchBeat_TrioKicker(&ctx, tobeat, beat, HAND_PRIMAL_PAIR);
      break;

    case Hand_Format(HAND_PRIMAL_TRIO, HAND_KICKER_SOLO, HAND_CHAINLESS):
      canbeat =
        _HandList_SearchBeat_TrioKicker(&ctx, tobeat, beat, HAND_PRIMAL_SOLO);
      break;

    case Hand_Format(HAND_PRIMAL_SOLO, HAND_KICKER_NONE, HAND_CHAIN):
      canbeat =
        _HandList_SearchBeat_Chain(&ctx, tobeat, beat, HAND_PRIMAL_SOLO);
      break;

    case Hand_Format(HAND_PRIMAL_PAIR, HAND_KICKER_NONE, HAND_CHAIN):
      canbeat =
        _HandList_SearchBeat_Chain(&ctx, tobeat, beat, HAND_PRIMAL_PAIR);
      break;

    case Hand_Format(HAND_PRIMAL_TRIO, HAND_KICKER_NONE, HAND_CHAIN):
      canbeat =
        _HandList_SearchBeat_Chain(&ctx, tobeat, beat, HAND_PRIMAL_TRIO);
      break;

    case Hand_Format(HAND_PRIMAL_FOUR, HAND_KICKER_NONE, HAND_CHAIN):
      canbeat =
        _HandList_SearchBeat_Chain(&ctx, tobeat, beat, HAND_PRIMAL_FOUR);
      break;

    case Hand_Format(HAND_PRIMAL_TRIO, HAND_KICKER_PAIR, HAND_CHAIN):
      canbeat = _HandList_SearchBeat_TrioKickerChain(&ctx,
                                                     tobeat,
                                                     beat,
                                                     HAND_PRIMAL_PAIR);
      break;

    case Hand_Format(HAND_PRIMAL_TRIO, HAND_KICKER_SOLO, HAND_CHAIN):
      canbeat = _HandList_SearchBeat_TrioKickerChain(&ctx,
                                                     tobeat,
                                                     beat,
                                                     HAND_PRIMAL_SOLO);
      break;

    default:
      break;
  }

  /* search for bomb/nuke */
  if (canbeat == 0) canbeat = _HandList_SearchBeat_Bomb(&ctx, tobeat, beat);

  return canbeat;
}

/*
 * search for beat, result will be store in beat
 * 1, if [beat->type] != 0, then search [new beat] > [beat]
 * 2, search [beat] > [tobeat], then store in [beat]
 */
int HandList_SearchBeat(card_array_t *cards, hand_t *tobeat, hand_t *beat) {
  /* already in search loop, continue */
  if (beat->type != 0)
    return _HandList_SearchBeat(cards, beat, beat);
  else
    return _HandList_SearchBeat(cards, tobeat, beat);
}

rk_list_t *HandList_SearchBeatList(card_array_t *cards, hand_t *tobeat) {
  rk_list_t *hl = NULL;
  hand_t htobeat;
  hand_t beat;
  int canbeat = 0;

  Hand_Clear(&beat);
  Hand_Copy(&htobeat, tobeat);

  hl = rk_list_create();
  do {
    canbeat = _HandList_SearchBeat(cards, &htobeat, &beat);

    if (canbeat) {
      Hand_Copy(&htobeat, &beat);
      HandList_PushFront(hl, &beat);
    }
  } while (canbeat);

  return hl;
}

/*
 * ************************************************************
 * hand analyzer
 * ************************************************************
 */

/*
 * extract hands like 34567 / 334455 / 333444555 etc
 * array is a processed card array holds count[rank] == duplicate
 */
void _HandList_ExtractConsecutive(rk_list_t *hl,
                                  card_array_t *array,
                                  int duplicate) {
  int i = 0;
  int j = 0;
  int k = 0;
  int cardnum = 0;
  uint8_t lastrank = 0;
  hand_t hand;
  int primal[] =
    {0, HAND_PRIMAL_SOLO, HAND_PRIMAL_PAIR, HAND_PRIMAL_TRIO};
  int chainlen[] =
    {0, HAND_SOLO_CHAIN_MIN_LENGTH, HAND_PAIR_CHAIN_MIN_LENGTH,
     HAND_TRIO_CHAIN_MIN_LENGTH};

  if ((duplicate < 1) || (duplicate > 3) || (array->length == 0)) return;

  Hand_Clear(&hand);

  cardnum = array->length / duplicate - 1;
  lastrank = CARD_RANK(array->cards[0]);
  i = duplicate;

  while (cardnum--) {
    if ((lastrank - 1) != CARD_RANK(array->cards[i])) {
      /* chain break */
      if (i >= chainlen[duplicate]) {
        /* chain */
        Hand_Clear(&hand);
        hand.type =
          Hand_Format(primal[duplicate], HAND_KICKER_NONE, HAND_CHAIN);

        for (j = 0; j < i; j++)
          CardArray_PushBack(&hand.cards, CardArray_PopFront(array));

        HandList_PushFront(hl, &hand);
      } else {
        /* not a chain */
        for (j = 0; j < i / duplicate; j++) {
          Hand_Clear(&hand);
          hand.type = Hand_Format(primal[duplicate],
                                  HAND_KICKER_NONE,
                                  HAND_CHAINLESS);

          for (k = 0; k < duplicate; k++)
            CardArray_PushBack(&hand.cards, CardArray_PopFront(array));

          HandList_PushFront(hl, &hand);
        }
      }

      if (array->length == 0) break;

      lastrank = CARD_RANK(array->cards[0]);
      i = duplicate;
    } else {
      /* chain intact */
      lastrank = CARD_RANK(array->cards[i]);
      i += duplicate;
    }
  }

  /* all chained up */
  if ((i != 0) && (i == array->length)) {
    /* can chain up */
    if (i >= chainlen[duplicate]) {
      Hand_Clear(&hand);
      hand.type = Hand_Format(primal[duplicate], HAND_KICKER_NONE, HAND_CHAIN);

      for (j = 0; j < i; j++)
        CardArray_PushBack(&hand.cards, CardArray_PopFront(array));

      HandList_PushFront(hl, &hand);
    } else {
      for (j = 0; j < i / duplicate; j++) {
        Hand_Clear(&hand);
        hand.type = Hand_Format(primal[duplicate],
                                HAND_KICKER_NONE,
                                HAND_CHAINLESS);

        for (k = 0; k < duplicate; k++)
          CardArray_PushBack(&hand.cards, CardArray_PopFront(array));

        HandList_PushFront(hl, &hand);
      }
    }
  }
}

/* extract nuke/bomb/2 from array, these cards will be removed from array */
void _HandList_ExtractNukeBomb2(rk_list_t *hl,
                                card_array_t *array,
                                int *count) {
  int i = 0;
  hand_t hand;

  /* nuke */
  if (count[CARD_RANK_r] && count[CARD_RANK_R]) {
    Hand_Clear(&hand);
    hand.type =
      Hand_Format(HAND_PRIMAL_NUKE, HAND_KICKER_NONE, HAND_CHAINLESS);
    CardArray_CopyRank(&hand.cards, array, CARD_RANK_R);
    CardArray_CopyRank(&hand.cards, array, CARD_RANK_r);

    HandList_PushFront(hl, &hand);

    count[CARD_RANK_r] = 0;
    count[CARD_RANK_R] = 0;

    CardArray_RemoveRank(array, CARD_RANK_r);
    CardArray_RemoveRank(array, CARD_RANK_R);
  }

  /* bomb */
  for (i = CARD_RANK_2; i >= CARD_RANK_3; i--) {
    if (count[i] == 4) {
      Hand_Clear(&hand);
      hand.type =
        Hand_Format(HAND_PRIMAL_BOMB, HAND_KICKER_NONE, HAND_CHAINLESS);
      CardArray_CopyRank(&hand.cards, array, (uint8_t) i);

      HandList_PushFront(hl, &hand);

      count[i] = 0;
      CardArray_RemoveRank(array, (uint8_t) i);
    }
  }

  /* joker */
  if ((count[CARD_RANK_r] != 0) || (count[CARD_RANK_R] != 0)) {
    Hand_Clear(&hand);
    CardArray_CopyRank(&hand.cards,
                       array,
                       count[CARD_RANK_r] != 0 ? CARD_RANK_r : CARD_RANK_R);
    hand.type =
      Hand_Format(HAND_PRIMAL_SOLO, HAND_KICKER_NONE, HAND_CHAINLESS);

    HandList_PushFront(hl, &hand);
    count[CARD_RANK_r] = 0;
    count[CARD_RANK_R] = 0;
    CardArray_RemoveRank(array, CARD_RANK_r);
    CardArray_RemoveRank(array, CARD_RANK_R);
  }

  /* 2 */
  if (count[CARD_RANK_2] != 0) {
    Hand_Clear(&hand);
    CardArray_CopyRank(&hand.cards, array, CARD_RANK_2);

    switch (count[CARD_RANK_2]) {
      case 1:
        hand.type =
          Hand_Format(HAND_PRIMAL_SOLO, HAND_KICKER_NONE, HAND_CHAINLESS);
        break;

      case 2:
        hand.type =
          Hand_Format(HAND_PRIMAL_PAIR, HAND_KICKER_NONE, HAND_CHAINLESS);
        break;

      case 3:
        hand.type =
          Hand_Format(HAND_PRIMAL_TRIO, HAND_KICKER_NONE, HAND_CHAINLESS);
        break;

      default:
        break;
    }
    count[CARD_RANK_2] = 0;
    CardArray_RemoveRank(array, CARD_RANK_2);
    HandList_PushFront(hl, &hand);
  }
}

rk_list_t *HandList_StandardAnalyze(card_array_t *cards) {
  int i = 0;
  int count[CARD_RANK_END];
  rk_list_t *hl = NULL;

  card_array_t array;
  card_array_t arrsolo;
  card_array_t arrpair;
  card_array_t arrtrio;
  card_array_t *arrprimals[3];

  CardArray_Clear(&arrsolo);
  CardArray_Clear(&arrpair);
  CardArray_Clear(&arrtrio);

  arrprimals[0] = &arrsolo;
  arrprimals[1] = &arrpair;
  arrprimals[2] = &arrtrio;

  CardArray_Copy(&array, cards);

  CardArray_Sort(&array, NULL);
  Hand_CountRank(&array, count, NULL);

  hl = rk_list_create();

  /* nuke, bomb and 2 */
  _HandList_ExtractNukeBomb2(hl, &array, count);

  /* chains */
  for (i = 0; i < array.length;) {
    int c = count[CARD_RANK(array.cards[i])];

    if (c != 0) {
      CardArray_PushBackCards(arrprimals[c - 1], &array, i, c);
      i += c;
    } else {
      i++;
    }
  }

  /* chain */
  _HandList_ExtractConsecutive(hl, &arrtrio, 3);
  _HandList_ExtractConsecutive(hl, &arrpair, 2);
  _HandList_ExtractConsecutive(hl, &arrsolo, 1);

  return hl;
}

/*
 * ************************************************************
 * hand evaluator
 * ************************************************************
 */
int _HandList_CalculateConsecutive(card_array_t *array, int duplicate) {
  int hands = 0;
  int i = 0;
  int j = 0;
  int k = 0;
  int cardnum = 0;
  uint8_t lastrank = 0;
  int chainlen[] =
    {0, HAND_SOLO_CHAIN_MIN_LENGTH, HAND_PAIR_CHAIN_MIN_LENGTH,
     HAND_TRIO_CHAIN_MIN_LENGTH};

  if ((duplicate < 1) || (duplicate > 3)) return hands;

  cardnum = array->length / duplicate;
  lastrank = CARD_RANK(array->cards[0]);
  i = duplicate;

  while (cardnum--) {
    if ((lastrank - 1) != CARD_RANK(array->cards[i])) {
      /* chain break */
      if (i >= chainlen[duplicate]) {
        /* chain */
        for (j = 0; j < i; j++)
          CardArray_PopFront(array);

        hands++;
      } else {
        /* not a chain */
        for (j = 0; j < i / duplicate; j++) {
          for (k = 0; k < duplicate; k++)
            CardArray_PopFront(array);

          hands++;
        }
      }

      if (array->length == 0) break;

      lastrank = CARD_RANK(array->cards[0]);
      i = duplicate;
    } else {
      /* chain intact */
      lastrank = CARD_RANK(array->cards[i]);
      i += duplicate;
    }
  }

  k = i - duplicate;                    /* step back */

  if ((k != 0) && (k == array->length)) /* all chained up */
  {
    /* can chain up */
    if (k >= chainlen[duplicate]) {
      for (j = 0; j < i - duplicate; j++)
        CardArray_PopFront(array);

      hands++;
    } else {
      for (j = 0; j < k / duplicate; j++) {
        for (i = 0; i < duplicate; i++)
          CardArray_PopFront(array);

        hands++;
      }
    }
  }

  return hands;
}

int HandList_StandardEvaluator(card_array_t *array) {
  int i = 0;
  int hands = 0;
  int count[CARD_RANK_END];

  card_array_t arrsolo;
  card_array_t arrpair;
  card_array_t arrtrio;
  card_array_t *arrkicks[3];

  arrkicks[0] = &arrsolo;
  arrkicks[1] = &arrpair;
  arrkicks[2] = &arrtrio;

  CardArray_Clear(&arrsolo);
  CardArray_Clear(&arrpair);
  CardArray_Clear(&arrtrio);

  CardArray_Sort(array, NULL);
  Hand_CountRank(array, count, NULL);

  /* nuke */
  if (count[CARD_RANK_r] && count[CARD_RANK_R]) {
    hands++;
    count[CARD_RANK_r] = 0;
    count[CARD_RANK_R] = 0;
  }

  /* bomb */
  for (i = CARD_RANK_2; i >= CARD_RANK_3; i--) {
    if (count[i] == 4) {
      hands++;
      count[i] = 0;
    }
  }

  /* joker */
  if ((count[CARD_RANK_r] != 0) || (count[CARD_RANK_R] != 0)) {
    hands++;
    count[CARD_RANK_r] = 0;
    count[CARD_RANK_R] = 0;
  }

  /* 2 */
  if (count[CARD_RANK_2] != 0) hands++;

  count[CARD_RANK_2] = 0;

  /* chains */
  for (i = 0; i < array->length;) {
    int c = count[CARD_RANK(array->cards[i])];

    if (c != 0) {
      CardArray_PushBackCards(arrkicks[c - 1], array, i, c);
      i += c;
    } else {
      i++;
    }
  }

  /* chain */
  hands += _HandList_CalculateConsecutive(&arrtrio, 3);
  hands += _HandList_CalculateConsecutive(&arrpair, 2);
  hands += _HandList_CalculateConsecutive(&arrsolo, 1);

  return hands;
}

/*
 * ************************************************************
 * advanced hand analyze
 * ************************************************************
 */
void _HandList_SearchLongestConsecutive(hand_ctx_t *ctx,
                                        hand_t *hand,
                                        int duplicate) {
  /* context */
  int i = 0;
  int j = 0;
  int k = 0;
  int rankstart = 0;
  uint8_t lastrank = 0;
  int primal[] =
    {0, HAND_PRIMAL_SOLO, HAND_PRIMAL_PAIR, HAND_PRIMAL_TRIO};
  int chainlen[] =
    {0, HAND_SOLO_CHAIN_MIN_LENGTH, HAND_PAIR_CHAIN_MIN_LENGTH,
     HAND_TRIO_CHAIN_MIN_LENGTH};
  card_array_t chain;
  int *count = ctx->count;
  card_array_t *cards = &ctx->rcards;

  if ((duplicate < 1) || (duplicate > 3)) return;

  /* early break */
  if (cards->length < chainlen[duplicate]) return;

  /* setup */
  CardArray_Clear(&chain);

  Hand_Clear(hand);
  rankstart = 0;

  /*
   * i <= CARD_RANK_2
   * but count[CARD_RANK_2] must be 0
   * for 2/bomb/nuke has been removed before calling this function
   */
  for (i = CARD_RANK_3; i <= CARD_RANK_2; i++) {
    /* find start of a possible chain */
    if (rankstart == 0) {
      if (count[i] >= duplicate) rankstart = i;

      continue;
    }

    /* chain break */
    if (count[i] < duplicate) {
      /* chain break, extract chain and set new possible start */
      if ((((i - rankstart) * duplicate) >= chainlen[duplicate]) &&
        ((i - rankstart) > chain.length)) {
        /* valid chain, store rank in card_array_t */
        CardArray_Clear(&chain);

        for (j = rankstart; j < i; j++) CardArray_PushBack(&chain, (uint8_t) j);
      }

      rankstart = 0;
    }
  }

  /* convert rank array to card array */
  if (chain.length > 0) {
    for (i = chain.length - 1; i >= 0; i--) {
      lastrank = chain.cards[i];
      k = duplicate;

      for (j = 0; j < cards->length; j++) {
        if (CARD_RANK(cards->cards[j]) == lastrank) {
          CardArray_PushBack(&hand->cards, cards->cards[j]);
          k--;

          if (k == 0) break;
        }
      }
    }

    hand->type = Hand_Format(primal[duplicate], HAND_KICKER_NONE, HAND_CHAIN);
  }
}

void _HandList_SearchPrimal(hand_ctx_t *ctx, hand_t *hand, int primal) {
  int i = 0;
  int *count = ctx->count;
  int primals[] =
    {0, HAND_PRIMAL_SOLO, HAND_PRIMAL_PAIR, HAND_PRIMAL_TRIO};
  card_array_t *rcards = &ctx->rcards;

  if ((primal > 3) || (primal < 1)) return;

  /* search count[rank] >= primal */
  for (i = 0; i < rcards->length; i++) {
    if (count[CARD_RANK(rcards->cards[i])] >= primal) {
      /* found */
      Hand_Clear(hand);
      CardArray_PushBackCards(&hand->cards, rcards, i, primal);
      hand->type =
        Hand_Format(primals[primal], HAND_KICKER_NONE, HAND_CHAINLESS);
      break;
    }
  }
}

typedef void (*_HandList_SearchPrimalFunc)(hand_ctx_t *,
                                           hand_t *,
                                           int);

#define HAND_SEARCH_TYPES 3

/*
 * pass a empty hand to start traverse
 * result stores in hand
 * return 0 when stop
 */
int _HLAA_TraverseChains(hand_ctx_t *ctx, int *begin, hand_t *hand) {
  int found = 0;
  int i = *begin;
  int primals[] = {1, 2, 3};

  /* solo chain, pair chain, trio chain, trio, pair, solo */
  _HandList_SearchPrimalFunc searchers[HAND_SEARCH_TYPES];

  searchers[0] = _HandList_SearchLongestConsecutive;
  searchers[1] = _HandList_SearchLongestConsecutive;
  searchers[2] = _HandList_SearchLongestConsecutive;

  if (ctx->cards.length == 0) return 0;

  if (*begin >= HAND_SEARCH_TYPES) return 0;

  /* init search */
  if (hand->type == 0) {
    while (i < HAND_SEARCH_TYPES && hand->type == 0) {
      searchers[i](ctx, hand, primals[i]);

      if (hand->type != 0) {
        found = 1;
        break;
      } else {
        i++;
        *begin = i;
      }
    }

    /* if found == 0, should PANIC */
  } else {
    /* continue search via beat */
    found = HandList_SearchBeat(&ctx->cards, hand, hand);
  }

  return found;
}

/*
 * extract all chains or primal hands in hand_ctx
 */
void _HLAA_ExtractAllChains(hand_ctx_t *ctx, rk_list_t *hands) {
  int found = 0;
  int lastsearch = 0;
  hand_t workinghand;
  hand_t lasthand;

  /* init search */
  Hand_Clear(&workinghand);
  Hand_Clear(&lasthand);

  found = _HLAA_TraverseChains(ctx, &lastsearch, &lasthand);

  while (found != 0) {
    HandList_PushFront(hands, &lasthand);

    Hand_Copy(&workinghand, &lasthand);

    while ((found =
              _HLAA_TraverseChains(ctx, &lastsearch, &workinghand)) != 0)
      HandList_PushFront(hands, &workinghand);

    /* can't find any more hands, try to reduce chain length */
    if (lasthand.type != 0) {
      if (lasthand.type ==
        Hand_Format(HAND_PRIMAL_SOLO, HAND_KICKER_NONE, HAND_CHAIN)) {
        if (lasthand.cards.length > HAND_SOLO_CHAIN_MIN_LENGTH) {
          CardArray_DropFront(&lasthand.cards, 1);
          found = 1;
        }
        else {
          lasthand.type = 0;
        }
      }
      else if (lasthand.type ==
        Hand_Format(HAND_PRIMAL_PAIR, HAND_KICKER_NONE, HAND_CHAIN)) {
        if (lasthand.cards.length > HAND_PAIR_CHAIN_MIN_LENGTH) {
          CardArray_DropFront(&lasthand.cards, 2);
          found = 1;
        }
        else {
          lasthand.type = 0;
        }
      }
      else if (lasthand.type ==
        Hand_Format(HAND_PRIMAL_TRIO, HAND_KICKER_NONE, HAND_CHAIN)) {
        if (lasthand.cards.length > HAND_TRIO_CHAIN_MIN_LENGTH) {
          CardArray_DropFront(&lasthand.cards, 3);
          found = 1;
        }
        else {
          lasthand.type = 0;
        }
      }

      /* still can't found, loop through hand type for more */
      if (found == 0) {
        lastsearch++;
        Hand_Clear(&lasthand);
        found = _HLAA_TraverseChains(ctx, &lastsearch, &lasthand);
      }
    }
  }
}

/* advanced search tree payload */
typedef struct _hltree_payload_s {
  /* hand context */
  hand_ctx_t ctx;
  /* hand */
  hand_t hand;
  /* evaluation weight */
  int weight;

} _hltree_payload_t;

rk_tree_t *_HLAA_TreeAddHand(rk_tree_t *tree, rk_list_node_t *handnode) {
  _hltree_payload_t *oldpayload = NULL;
  _hltree_payload_t *newpayload = NULL;

  oldpayload = (_hltree_payload_t *) tree->payload;
  newpayload = (_hltree_payload_t *) malloc(sizeof(_hltree_payload_t));

  /* make diff here */
  memcpy(&newpayload->ctx, &oldpayload->ctx, sizeof(hand_ctx_t));
  Hand_Copy(&newpayload->hand, HandList_GetHand(handnode));
  CardArray_Subtract(&newpayload->ctx.cards, &HandList_GetHand(handnode)->cards);
  CardArray_Copy(&newpayload->ctx.rcards, &newpayload->ctx.cards);
  CardArray_Reverse(&newpayload->ctx.rcards);
  Hand_CountRank(&newpayload->ctx.cards, newpayload->ctx.count, NULL);
  newpayload->weight = oldpayload->weight + 1;

  /* expand the tree */
  return rk_tree_add_child(tree, newpayload);
}

/*
 * search hand via least hands
 */
rk_list_t *HandList_AdvancedAnalyze(card_array_t *array) {
  rk_list_t *handlist = NULL;
  rk_list_t *chains = NULL;
  rk_list_t *others = NULL;
  rk_list_node_t *hlnode = NULL;
  rk_list_t *st = NULL;
  rk_tree_t *grandtree = NULL;
  rk_tree_t *workingtree = NULL;
  rk_tree_t *tnode = NULL;
  rk_tree_t *shortest = NULL;
  _hltree_payload_t *pload = NULL;

  hand_ctx_t ctx;

  handlist = rk_list_create();

  /* setup search context */
  HandCtx_Clear(&ctx);

  /* build beat search context */
  Hand_CountRank(array, ctx.count, NULL);
  CardArray_Copy(&ctx.cards, array);

  /* extract bombs and 2 */
  _HandList_ExtractNukeBomb2(handlist, &ctx.cards, ctx.count);

  /* finish building beat_search_context */
  CardArray_Copy(&ctx.rcards, &ctx.cards);
  CardArray_Reverse(&ctx.rcards);

  /* magic goes here */

  /* root */
  pload = (_hltree_payload_t *) malloc(sizeof(_hltree_payload_t));
  memcpy(&pload->ctx, &ctx, sizeof(hand_ctx_t));
  pload->weight = 0;
  grandtree = rk_tree_create(pload);

  /* first expansion */
  chains = rk_list_create();
  _HLAA_ExtractAllChains(&ctx, chains);

  /* no chains, fall back to standard analyze */
  if (rk_list_empty(chains)) {
    rk_list_clear_destroy(handlist);
    rk_list_clear_destroy(chains);
    rk_tree_clear_destroy(grandtree);
    return HandList_StandardAnalyze(array);
  }

  /* got chains, make first expand */
  hlnode = chains->first;
  st = rk_list_create();

  while (hlnode != NULL) {
    tnode = _HLAA_TreeAddHand(grandtree, hlnode);
    rk_list_push(st, tnode);

    hlnode = hlnode->next;
  }

  rk_list_clear_destroy(chains);

  /* loop start */
  while (!rk_list_empty(st)) {
    /* pop stack */
    workingtree = rk_list_pop(st);
    chains = rk_list_create();
    pload = (_hltree_payload_t *) workingtree->payload;

    /* expansion */
    _HLAA_ExtractAllChains(&pload->ctx, chains);

    if (!rk_list_empty(chains)) {
      /* push new nodes */
      hlnode = chains->first;

      while (hlnode != NULL) {
        tnode = _HLAA_TreeAddHand(workingtree, hlnode);
        rk_list_push(st, tnode);

        hlnode = hlnode->next;
      }
    }

    rk_list_clear_destroy(chains);
  }

  /* tree construction complete */
  rk_tree_dump_leaves(grandtree, st);

  /* find shortest path */
  while (!rk_list_empty(st)) {
    /* pop stack */
    workingtree = rk_list_pop(st);
    pload = (_hltree_payload_t *) workingtree->payload;

    /* calculate other hands weight */
    pload->weight += HandList_StandardEvaluator(&pload->ctx.cards);

    if ((shortest == NULL) ||
      (pload->weight < ((_hltree_payload_t *) shortest->payload)->weight))
      shortest = workingtree;
  }

  rk_list_clear_destroy(st);

  /* extract shortest node's other hands */
  others =
    HandList_StandardAnalyze(&((_hltree_payload_t *) (shortest->payload))->ctx.cards);

  while (shortest != NULL &&
    ((_hltree_payload_t *) shortest->payload)->weight != 0) {
    HandList_PushFront(others,
                       &((_hltree_payload_t *) (shortest->payload))->hand);
    shortest = shortest->parent;
  }

  rk_list_concat(others, handlist);
  handlist->first = NULL;
  handlist->last = NULL;
  rk_list_destroy(handlist);

  rk_tree_clear_destroy(grandtree);

  return others;
}

/* TODO */
int HandList_AdvancedEvaluator(card_array_t *array) {
  int value = 0;
  rk_list_t *hl = HandList_AdvancedAnalyze(array);
  value = rk_list_count(hl);
  rk_list_clear_destroy(hl);

  return value;
}

/*
 * ************************************************************
 * best beat
 * ************************************************************
 */

#define BEAT_VALUE_FACTOR 10

/* nodes for beat list sort */
typedef struct beat_node_s {
  hand_t *hand;
  int value;

} beat_node_t;

/* sort function */
int _BeatNode_ValueSort(const void *a, const void *b) {
  return ((*(beat_node_t **) a)->value - (*(beat_node_t **) b)->value);
}

int HandList_BestBeat(card_array_t *array,
                      hand_t *tobeat,
                      hand_t *beat,
                      HandList_EvaluateFunc func) {
  int i = 0;
  int nodei = 0;
  int bombi = 0;
  int canbeat = 0;
  rk_list_t *hl = NULL;
  rk_list_node_t *node = NULL;
  card_array_t temp;
  beat_node_t *hnodes[BEAT_NODE_CAPACITY];
  hand_t *hbombs[BEAT_NODE_CAPACITY];
  HandList_EvaluateFunc evalFunc;

  evalFunc = (func == NULL) ? HandList_StandardEvaluator : func;

  memset(hnodes, 0, sizeof(beat_node_t *) * BEAT_NODE_CAPACITY);
  memset(hbombs, 0, sizeof(hand_t *) * BEAT_NODE_CAPACITY);

  /* search beat list */
  hl = HandList_SearchBeatList(array, tobeat);

  /* separate bomb/nuke and normal hands */
  node = hl->first;

  while (node != NULL) {
    if ((HandList_GetHand(node)->type ==
      Hand_Format(HAND_PRIMAL_BOMB, HAND_KICKER_NONE, HAND_CHAINLESS)) ||
      (HandList_GetHand(node)->type ==
        Hand_Format(HAND_PRIMAL_NUKE, HAND_KICKER_NONE, HAND_CHAINLESS))) {
      hbombs[bombi++] = node->payload;
    }
    else {
      hnodes[nodei] = (beat_node_t *) malloc(sizeof(beat_node_t));
      hnodes[nodei]->hand = node->payload;
      nodei++;
    }

    node = node->next;
  }

  /* calculate value */
  if (nodei > 1) {
    for (i = 0; i < nodei; i++) {
      hand_t *leftover;
      CardArray_Copy(&temp, array);

      /* evaluate the value of cards left after hand was played */
      leftover = hnodes[i]->hand;
      CardArray_Subtract(&temp, &leftover->cards);

      hnodes[i]->value = evalFunc(&temp) *
        BEAT_VALUE_FACTOR +
        CARD_RANK(leftover->cards.cards[0]);
    }

    /* sort primal hands */
    qsort(hnodes, nodei, sizeof(beat_node_t *), _BeatNode_ValueSort);
  }

  /* re-build hand list */
  rk_list_destroy(hl);
  hl = rk_list_create();

  for (i = bombi; i >= 0; i--) {
    if (hbombs[i]) {
      rk_list_push(hl, hbombs[i]);
    }
  }

  for (i = nodei; i >= 0; i--) {
    if (hnodes[i]) {
      rk_list_push(hl, hnodes[i]->hand);
    }
  }

  /* select beat */
  if (!rk_list_empty(hl)) {
    Hand_Copy(beat, HandList_GetHand(hl->first));
    canbeat = 1;
  }

  /* clean up */
  for (i = 0; i < nodei; i++) free(hnodes[i]);

  rk_list_clear_destroy(hl);

  return canbeat;
}

void HandList_Print(rk_list_t *hl) {
  rk_list_node_t *node = NULL;

  if (hl == NULL || rk_list_empty(hl) || HandList_GetHand(hl->first)->type == 0)
    return;

  DBGLog ("-----hand_list_t begin---------\n");
  for (node = hl->first; node != NULL; node = node->next) {
    Hand_Print(HandList_GetHand(node));
  }
  DBGLog ("-----hand_list_t ended---------\n");
}
