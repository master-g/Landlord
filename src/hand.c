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
 * brief about Hand_Parse function
 * --------------------------------------
 *
 * Landlord has few types of hands,
 * for example, solo-chain as 34567, trio-solo-chain as 333444555A26
 * their patterns are 11111, 333111
 * which can help us to determine the type of a specific hand
 *
 * for example, a 6 cards can have 4 types of hands
 * solo-chain, pair-chain, trio-chain and four-dual-solo
 *
 * the parse process can be simply describe as
 * 1. rank count            --  count every rank in the card array
 * 2. rank count sort       --  sort the count result
 * 3. pattern match         --  iterate through possible patterns
 */

#include "hand.h"

#define HAND_VARIATION              2
#define HAND_SPEC                   4

/* ************************************************************
 * pattern
 * ************************************************************/

#define PATTERN_LENGTH 12

#define HAND_PATTERN_NONE  0  /* place holder */
#define HAND_PATTERN_1     1  /* 1, solo */
#define HAND_PATTERN_2_1   2  /* 2, pair */
#define HAND_PATTERN_2_2   3  /* 2, nuke */
#define HAND_PATTERN_3     4  /* 3, trio */
#define HAND_PATTERN_4_1   5  /* bomb */
#define HAND_PATTERN_4_2   6  /* trio solo */
#define HAND_PATTERN_5_1   7  /* solo chain */
#define HAND_PATTERN_5_2   8  /* trio pair */
#define HAND_PATTERN_6_1   9  /* solo chain */
#define HAND_PATTERN_6_2   10 /* pair chain */
#define HAND_PATTERN_6_3   11 /* trio chain */
#define HAND_PATTERN_6_4   12 /* four dual solo */
#define HAND_PATTERN_7     13 /* solo chain */
#define HAND_PATTERN_8_1   14 /* solo chain */
#define HAND_PATTERN_8_2   15 /* pair chain */
#define HAND_PATTERN_8_3   16 /* trio solo chain */
#define HAND_PATTERN_8_4   17 /* four dual pair */
#define HAND_PATTERN_8_5   18 /* four chain */
#define HAND_PATTERN_9_1   19 /* solo chain */
#define HAND_PATTERN_9_2   20 /* trio chain */
#define HAND_PATTERN_10_1  21 /* solo chain */
#define HAND_PATTERN_10_2  22 /* pair chain */
#define HAND_PATTERN_10_3  23 /* trio pair chain */
#define HAND_PATTERN_11    24 /* solo chain */
#define HAND_PATTERN_12_1  25 /* solo chain */
#define HAND_PATTERN_12_2  26 /* pair chain */
#define HAND_PATTERN_12_3  27 /* trio chain */
#define HAND_PATTERN_12_4  28 /* trio solo chain */
#define HAND_PATTERN_12_5  29 /* four chain */
#define HAND_PATTERN_12_6  30 /* four dual solo chain */
#define HAND_PATTERN_14    31 /* pair chain */
#define HAND_PATTERN_15    32 /* trio chain */
#define HAND_PATTERN_16_1  33 /* pair chain */
#define HAND_PATTERN_16_2  34 /* trio solo chain */
#define HAND_PATTERN_16_3  35 /* four chain */
#define HAND_PATTERN_16_4  36 /* four dual pair chain */
#define HAND_PATTERN_18_1  37 /* pair chain */
#define HAND_PATTERN_18_2  38 /* trio chain */
#define HAND_PATTERN_18_3  39 /* four dual solo chain */
#define HAND_PATTERN_20_1  40 /* pair chain */
#define HAND_PATTERN_20_2  41 /* trio solo chain */
#define HAND_PATTERN_20_3  42 /* four chain */
#define HAND_PATTERN_END   HAND_PATTERN_20_3

const int _hand_pattern[][PATTERN_LENGTH] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* place holder */
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* 1, solo */
  {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* 2, pair */
  {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* 2, nuke */
  {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* 3, trio */
  {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* 4, bomb */
  {3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* 4, trio solo */
  {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0}, /* 5, solo chain */
  {3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* 5, trio pair */
  {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, /* 6, solo chain */
  {2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* 6, pair chain */
  {3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* 6, trio chain */
  {4, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* 6, four dual solo */
  {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}, /* 7, solo chain */
  {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0}, /* 8, solo chain */
  {2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0}, /* 8, pair chain */
  {3, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, /* 8, trio solo chain */
  {4, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* 8, four dual pair */
  {4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* 8, four chain */
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}, /* 9, solo chain */
  {3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* 9, trio chain */
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0}, /* 10, solo chain */
  {2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0}, /* 10, pair chain */
  {3, 3, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0}, /* 10, trio pair chain */
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}, /* 11, solo chain */
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, /* 12, solo chain */
  {2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0}, /* 12, pair chain */
  {3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0}, /* 12, trio chain */
  {3, 3, 3, 1, 1, 1, 0, 0, 0, 0, 0, 0}, /* 12, trio solo chain */
  {4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* 12, four chain */
  {4, 4, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, /* 12, four dual solo chain */
  {2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0}, /* 14, pair chain */
  {3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0}, /* 15, trio chain */
  {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0}, /* 16, pair chain */
  {3, 3, 3, 3, 1, 1, 1, 1, 0, 0, 0, 0}, /* 16, trio solo chain */
  {4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0}, /* 16, four chain */
  {4, 4, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0}, /* 16, four dual pair chain */
  {2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0}, /* 18, pair chain */
  {3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0}, /* 18, trio chain */
  {4, 4, 4, 1, 1, 1, 1, 1, 1, 0, 0, 0}, /* 18, four dual solo chain */
  {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0}, /* 20, pair chain */
  {3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 0, 0}, /* 20, trio solo chain */
  {4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0}, /* 20, four chain */
};

/**
 *  hand specification contains 5 elements
 *  --------------------------------------
 *  pattern
 *  length (contains in card_array_t)
 *  primal
 *  kicker
 *  chain
 *
 *  except solo/pair/trio chains
 *  hands that had same length may have 2 variations at most
 */

const int _hand_specs[HAND_MAX_LENGTH + 1][HAND_VARIATION][HAND_SPEC] = {
  { /* place holder */
    {0, 0, 0, 0}, {0, 0, 0, 0}
  }, { /* 1 */
    {HAND_PATTERN_1, HAND_PRIMAL_SOLO, 0, 0},
    {0, 0, 0, 0}
  }, { /* 2 */
    {HAND_PATTERN_2_1, HAND_PRIMAL_PAIR, 0, 0},
    {0, 0, 0, 0}
  }, { /* 3 */
    {HAND_PATTERN_3, HAND_PRIMAL_TRIO, 0, 0},
    {0, 0, 0, 0}
  }, { /* 4 */
    {HAND_PATTERN_4_2, HAND_PRIMAL_TRIO, HAND_KICKER_SOLO, 0},
    {0, 0, 0, 0}
  }, { /* 5 */
    {HAND_PATTERN_5_2, HAND_PRIMAL_TRIO, HAND_KICKER_PAIR, 0},
    {0, 0, 0, 0}
  }, { /* 6 */
    {HAND_PATTERN_6_4, HAND_PRIMAL_FOUR, HAND_KICKER_DUAL_SOLO, 0},
    {0, 0, 0, 0}
  }, { /* 7 */
    {0, 0, 0, 0}, {0, 0, 0, 0}
  }, { /* 8 */
    {HAND_PATTERN_8_3, HAND_PRIMAL_TRIO, HAND_KICKER_SOLO, HAND_CHAIN},
    {HAND_PATTERN_8_4, HAND_PRIMAL_FOUR, HAND_KICKER_DUAL_PAIR, 0}
  }, { /* 9 */
    {0, 0, 0, 0}, {0, 0, 0, 0}
  }, { /* 10 */
    {HAND_PATTERN_10_3, HAND_PRIMAL_TRIO, HAND_KICKER_PAIR, HAND_CHAIN},
    {0, 0, 0, 0}
  }, { /* 11 */
    {0, 0, 0, 0}, {0, 0, 0, 0}
  }, { /* 12 */
    {HAND_PATTERN_12_4, HAND_PRIMAL_TRIO, HAND_KICKER_SOLO, HAND_CHAIN},
    {HAND_PATTERN_12_6, HAND_PRIMAL_FOUR, HAND_KICKER_DUAL_SOLO, HAND_CHAIN}
  }, { /* 13 */
    {0, 0, 0, 0}, {0, 0, 0, 0}
  }, { /* 14 */
    {0, 0, 0, 0}, {0, 0, 0, 0}
  }, { /* 15 */
    {0, 0, 0, 0}, {0, 0, 0, 0}
  }, { /* 16 */
    {HAND_PATTERN_16_2, HAND_PRIMAL_TRIO, HAND_KICKER_SOLO, HAND_CHAIN},
    {HAND_PATTERN_16_4, HAND_PRIMAL_FOUR, HAND_KICKER_DUAL_PAIR, HAND_CHAIN}
  }, { /* 17 */
    {0, 0, 0, 0}, {0, 0, 0, 0}
  }, { /* 18 */
    {HAND_PATTERN_18_3, HAND_PRIMAL_FOUR, HAND_KICKER_DUAL_SOLO,
     HAND_CHAIN}, {0, 0, 0, 0}
  }, { /* 19 */
    {0, 0, 0, 0}, {0, 0, 0, 0}
  }, { /* 20 */
    {HAND_PATTERN_20_2, HAND_PRIMAL_TRIO, HAND_KICKER_SOLO, HAND_CHAIN},
    {0, 0, 0, 0}
  }
};

/* ************************************************************
 * hand
 * ************************************************************/
void Hand_Clear(hand_t *hand) {
  CardArray_Clear(&hand->cards);
  hand->type = 0;
}

void Hand_Copy(hand_t *dst, hand_t *src) {
  dst->type = src->type;
  CardArray_Copy(&dst->cards, &src->cards);
}

/* ************************************************************
 * parser
 * ************************************************************/

/* sort function for rank count array */
int _Hand_PatternSort(const void *a, const void *b) {
  return *(int *) b - *(int *) a;
}

/* sort count array by counts */
void _Hand_SortCount(int *count) {
  qsort(count, CARD_RANK_END, sizeof(int), _Hand_PatternSort);
}

/*
 * count rank in card array
 * count[rank] = num
 */
void Hand_CountRank(card_array_t *array, int *count, int *sort) {
  int i = 0;

  memset(count, 0, sizeof(int) * CARD_RANK_END);

  for (i = 0; i < array->length; i++)
    count[CARD_RANK(array->cards[i])]++;

  if (sort != NULL) {
    memcpy(sort, count, sizeof(int) * CARD_RANK_END);
    _Hand_SortCount(sort);
  }
}

/* check if a sorted count array matches specific pattern */
int _Hand_PatternMatch(int *sorted, int pattern) {
  int ret =
    memcmp(sorted, _hand_pattern[pattern], sizeof(int) * PATTERN_LENGTH) == 0
    ? 1 : 0;

  return ret;
}

/*
 * check pattern as 334455 666777 etc
 * | 666 | 777 | 888 | 999 |
 * | 123 |                   duplicate: 3
 * |  1     2     3     4  | expectLength: 4
 */
int _Hand_CheckChain(int *count, int duplicate, int expectLength) {
  int i = 0;
  int marker = 0;
  int length = 0;

  /* joker and 2 can't chain up */
  for (i = CARD_RANK_3; i < CARD_RANK_2; i++) {
    /* found first match */
    if ((count[i] == duplicate) && (marker == 0)) {
      marker = i;
      continue;
    }

    /* matches end */
    if (((count[i] != duplicate) && (marker != 0))) {
      length = i - marker;
      break;
    }
  }

  return length == expectLength ? 1 : 0;
}

/*
 * distribute cards
 * for example, _Hand_Distribute(xxx, 88666644, 422, 4, 2, 8)
 * hand will be 66668844
 */
void _Hand_Distribute(hand_t *hand,
                      card_array_t *array,
                      int *count,
                      int d1,
                      int d2,
                      int length) {
  int i = 0;
  int num = 0;
  uint8_t card = 0;
  card_array_t temp;

  CardArray_Clear(&temp);

  for (i = 0; i < array->length; i++) {
    card = array->cards[i];
    num = count[CARD_RANK(card)];

    if (num == d1)
      CardArray_PushBack(&hand->cards, card);
    else if (num == d2)
      CardArray_PushBack(&temp, card);

    if (hand->cards.length + temp.length >= length) {
      CardArray_Concat(&hand->cards, &temp);
      break;
    }
  }
}

int _Hand_CheckNuke(hand_t *hand,
                    card_array_t *array,
                    int *count,
                    int *sorted) {
  int ret = 0;
  if ((CARD_RANK(array->cards[0]) == CARD_RANK_R) &&
    (CARD_RANK(array->cards[1]) == CARD_RANK_r)) {
    hand->type =
      Hand_Format(HAND_PRIMAL_NUKE, HAND_KICKER_NONE, HAND_CHAINLESS);
    CardArray_Copy(&hand->cards, array);
    ret = hand->type;
  }

  return ret;
}

int _Hand_CheckBomb(hand_t *hand,
                    card_array_t *array,
                    int *count,
                    int *sorted) {
  int ret = 0;

  if (_Hand_PatternMatch(sorted, HAND_PATTERN_4_1)) {
    /* bomb, 4 */
    CardArray_Copy(&hand->cards, array);
    hand->type =
      Hand_Format(HAND_PRIMAL_BOMB, HAND_KICKER_NONE, HAND_CHAINLESS);
    ret = hand->type;
  }

  return ret;
}

int Hand_Parse(hand_t *hand, card_array_t *array) {
  int count[CARD_RANK_END];
  int sorted[CARD_RANK_END];
  int chainlength[HAND_PRIMAL_FOUR + 1] = {0, HAND_SOLO_CHAIN_MIN_LENGTH,
                                           HAND_PAIR_CHAIN_MIN_LENGTH,
                                           HAND_TRIO_CHAIN_MIN_LENGTH,
                                           HAND_FOUR_CHAIN_MIN_LENGTH};

  /* sort cards */
  CardArray_Sort(array, NULL);

  /* count ranks */
  Hand_CountRank(array, count, sorted);

  /* clear hand */
  Hand_Clear(hand);

  do {
    int i = 0;

    /* validate length */
    if ((array->length < HAND_MIN_LENGTH)
      || (array->length > HAND_MAX_LENGTH)) {
      hand->type = HAND_NONE;
      break;
    }

    /* nuke */
    if ((array->length == 2) && _Hand_CheckNuke(hand, array, count, sorted)) {
      break;
    }

    /* bomb */
    if ((array->length == 4) && _Hand_CheckBomb(hand, array, count, sorted)) {
      break;
    }

    /* chains */
    for (i = 1; i < HAND_PRIMAL_FOUR + 1; i++) {
      int chainMinLength = chainlength[i];
      if ((array->length >= chainMinLength) && (array->length % i == 0)
        && _Hand_CheckChain(count, i, array->length / i)) {
        hand->type = Hand_Format(i, HAND_KICKER_NONE, HAND_CHAIN);
        CardArray_Copy(&hand->cards, array);
        break;
      }
    }

    /* chain or other type */
    if (hand->type != 0) {
      break;
    } else {
      int pattern, primal, kicker, chain, d1;
      int d2[] = {0, 1, 2, 1, 2};

      for (i = 0; i < 2; i++) {
        pattern = _hand_specs[array->length][i][0];
        primal = _hand_specs[array->length][i][1];
        kicker = _hand_specs[array->length][i][2];
        chain = _hand_specs[array->length][i][3];

        if (pattern == 0) {
          hand->type = 0;
          break;
        }

        if (_Hand_PatternMatch(sorted, pattern)) {
          d1 = primal;
          _Hand_Distribute(hand,
                           array,
                           count,
                           d1,
                           d2[kicker >> 4],
                           array->length);
          hand->type = Hand_Format(primal, kicker, chain);
          break;
        }
      }
    }

  } while (0);

  return hand->type;
}

/*
 * ************************************************************
 * comparators
 * ************************************************************
 */

/* one of a, b must be bomb or nuke */
int _Hand_CompareBomb(hand_t *a, hand_t *b) {
  int ret = HAND_CMP_ILLEGAL;

  /* same type same cards, equal */
  if ((a->type == b->type) &&
    (a->cards.cards[0] == b->cards.cards[0]))
    ret = HAND_CMP_EQUAL;

    /* both are bombs, compare by card rank */
  else if ((a->type == HAND_PRIMAL_BOMB) &&
    (b->type == HAND_PRIMAL_BOMB))
    ret = CARD_RANK(a->cards.cards[0]) >
      CARD_RANK(
        b->cards.cards[0]) ? HAND_CMP_GREATER : HAND_CMP_LESS;
  else
    ret = Hand_GetPrimal(a->type) >
      Hand_GetPrimal(b->type) ? HAND_CMP_GREATER : HAND_CMP_LESS;

  return ret;
}

int Hand_Compare(hand_t *a, hand_t *b) {
  int result = HAND_CMP_ILLEGAL;

  /*
   * different hand type
   * check for bomb and nuke
   */
  if (a->type != b->type) {
    if ((a->type != HAND_PRIMAL_NUKE) &&
      (a->type != HAND_PRIMAL_BOMB) &&
      (b->type != HAND_PRIMAL_NUKE) &&
      (b->type != HAND_PRIMAL_BOMB))
      result = HAND_CMP_ILLEGAL;
    else result = _Hand_CompareBomb(a, b);
  }
  else /* same hand type and with no bombs */
  {
    /* same hand type but different length */
    if (a->cards.length != b->cards.length) {
      result = HAND_CMP_ILLEGAL;
    }
    else /* same hand type and same length */
    {
      if (CARD_RANK(a->cards.cards[0]) ==
        CARD_RANK(b->cards.cards[0]))
        result = HAND_CMP_EQUAL;
      else
        result = CARD_RANK(a->cards.cards[0]) >
          CARD_RANK(b->cards.cards[0]) ? HAND_CMP_GREATER :
                 HAND_CMP_LESS;
    }
  }

  return result;
}

void Hand_Print(hand_t *hand) {
  const char *toprint = "";

  DBGLog ("Hand type: [");

  if (hand == NULL) {
    DBGLog ("null\n");
    return;
  }

  switch (Hand_GetPrimal(hand->type)) {
    case HAND_PRIMAL_NONE:
      toprint = "none ";
      break;

    case HAND_PRIMAL_SOLO:
      toprint = "solo ";
      break;

    case HAND_PRIMAL_PAIR:
      toprint = "pair ";
      break;

    case HAND_PRIMAL_TRIO:
      toprint = "trio ";
      break;

    case HAND_PRIMAL_FOUR:
      toprint = "four ";
      break;

    case HAND_PRIMAL_BOMB:
      toprint = "bomb ";
      break;

    case HAND_PRIMAL_NUKE:
      toprint = "nuke ";
      break;

    default:
      break;
  }

  DBGLog ("%s", toprint);

  switch (Hand_GetKicker(hand->type)) {
    case HAND_KICKER_NONE:
      toprint = NULL;
      break;

    case HAND_KICKER_SOLO:
      toprint = "solo";
      break;

    case HAND_KICKER_PAIR:
      toprint = "pair";
      break;

    case HAND_KICKER_DUAL_SOLO:
      toprint = "dual solo";
      break;

    case HAND_KICKER_DUAL_PAIR:
      toprint = "dual pair";
      break;

    default:
      toprint = NULL;
      break;
  }

  if (toprint != NULL) DBGLog ("%s", toprint);

  if (Hand_GetChain(hand->type) == HAND_CHAIN) DBGLog (" chain");

  DBGLog ("]\n");

  CardArray_Print(&hand->cards);
}
