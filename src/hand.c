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
 * for exampe, a 6 cards can have 4 types of hands
 * solo-chain, pair-chain, trio-chain and four-dual-solo
 *
 * the parse process can be simply describe as
 * 1. rank count            --  count every rank in the card array
 * 2. rank count sort       --  sort the count result
 * 3. pattern match         --  iterate through possible patterns
 */

#include "hand.h"

#define HAND_MIN_LENGTH             1
#define HAND_MAX_LENGTH             20
#define HAND_VARIATION              2
#define HAND_SPEC                   4
#define HAND_SOLO_CHAIN_MIN_LENGTH  5
#define HAND_PAIR_CHAIN_MIN_LENGTH  6
#define HAND_TRIO_CHAIN_MIN_LENGTH  6
#define HAND_FOUR_CHAIN_MIN_LENGTH  8

#define BEAT_NODE_CAPACITY          255

/*
 * ************************************************************
 * pattern
 * ************************************************************
 */

#define PATTERN_LENGTH 12

#define HANDS_PATTERN_NONE  0  /* place holder */
#define HANDS_PATTERN_1     1  /* 1, solo */
#define HANDS_PATTERN_2_1   2  /* 2, pair */
#define HANDS_PATTERN_2_2   3  /* 2, nuke */
#define HANDS_PATTERN_3     4  /* 3, trio */
#define HANDS_PATTERN_4_1   5  /* bomb */
#define HANDS_PATTERN_4_2   6  /* trio solo */
#define HANDS_PATTERN_5_1   7  /* solo chain */
#define HANDS_PATTERN_5_2   8  /* trio pair */
#define HANDS_PATTERN_6_1   9  /* solo chain */
#define HANDS_PATTERN_6_2   10 /* pair chain */
#define HANDS_PATTERN_6_3   11 /* trio chain */
#define HANDS_PATTERN_6_4   12 /* four dual solo */
#define HANDS_PATTERN_7     13 /* solo chain */
#define HANDS_PATTERN_8_1   14 /* solo chain */
#define HANDS_PATTERN_8_2   15 /* pair chain */
#define HANDS_PATTERN_8_3   16 /* trio solo chain */
#define HANDS_PATTERN_8_4   17 /* four dual pair */
#define HANDS_PATTERN_8_5   18 /* four chain */
#define HANDS_PATTERN_9_1   19 /* solo chain */
#define HANDS_PATTERN_9_2   20 /* trio chain */
#define HANDS_PATTERN_10_1  21 /* solo chain */
#define HANDS_PATTERN_10_2  22 /* pair chain */
#define HANDS_PATTERN_10_3  23 /* trio pair chain */
#define HANDS_PATTERN_11    24 /* solo chain */
#define HANDS_PATTERN_12_1  25 /* solo chain */
#define HANDS_PATTERN_12_2  26 /* pair chain */
#define HANDS_PATTERN_12_3  27 /* trio chain */
#define HANDS_PATTERN_12_4  28 /* trio solo chain */
#define HANDS_PATTERN_12_5  29 /* four chain */
#define HANDS_PATTERN_12_6  30 /* four dual solo chain */
#define HANDS_PATTERN_14    31 /* pair chain */
#define HANDS_PATTERN_15    32 /* trio chain */
#define HANDS_PATTERN_16_1  33 /* pair chain */
#define HANDS_PATTERN_16_2  34 /* trio solo chain */
#define HANDS_PATTERN_16_3  35 /* four chain */
#define HANDS_PATTERN_16_4  36 /* four dual pair chain */
#define HANDS_PATTERN_18_1  37 /* pair chain */
#define HANDS_PATTERN_18_2  38 /* trio chain */
#define HANDS_PATTERN_18_3  39 /* four dual solo chain */
#define HANDS_PATTERN_20_1  40 /* pair chain */
#define HANDS_PATTERN_20_2  41 /* trio solo chain */
#define HANDS_PATTERN_20_3  42 /* four chain */
#define HANDS_PATTERN_END   HANDS_PATTERN_20_3


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
        {HANDS_PATTERN_1, HAND_PRIMAL_SOLO, 0, 0},
        {0, 0, 0, 0}
    }, { /* 2 */
        {HANDS_PATTERN_2_1, HAND_PRIMAL_PAIR, 0, 0},
        {0, 0, 0, 0}
    }, { /* 3 */
        {HANDS_PATTERN_3, HAND_PRIMAL_TRIO, 0, 0},
        {0, 0, 0, 0}
    }, { /* 4 */
        {HANDS_PATTERN_4_2, HAND_PRIMAL_TRIO, HAND_KICKER_SOLO, 0},
        {0, 0, 0, 0}
    }, { /* 5 */
        {HANDS_PATTERN_5_2, HAND_PRIMAL_TRIO, HAND_KICKER_PAIR, 0},
        {0, 0, 0, 0}
    }, { /* 6 */
        {HANDS_PATTERN_6_4, HAND_PRIMAL_FOUR, HAND_KICKER_DUAL_SOLO, 0},
        {0, 0, 0, 0}
    }, { /* 7 */
        {0, 0, 0, 0}, {0, 0, 0, 0}
    }, { /* 8 */
        {HANDS_PATTERN_8_3, HAND_PRIMAL_TRIO, HAND_KICKER_SOLO, HAND_CHAIN},
        {HANDS_PATTERN_8_4, HAND_PRIMAL_FOUR, HAND_KICKER_DUAL_PAIR, 0}
    }, { /* 9 */
        {0, 0, 0, 0}, {0, 0, 0, 0}
    }, { /* 10 */
        {HANDS_PATTERN_10_3, HAND_PRIMAL_TRIO, HAND_KICKER_PAIR, HAND_CHAIN},
        {0, 0, 0, 0}
    }, { /* 11 */
        {0, 0, 0, 0}, {0, 0, 0, 0}
    }, { /* 12 */
        {HANDS_PATTERN_12_4, HAND_PRIMAL_TRIO, HAND_KICKER_SOLO, HAND_CHAIN},
        {HANDS_PATTERN_12_6, HAND_PRIMAL_FOUR, HAND_KICKER_DUAL_SOLO, HAND_CHAIN}
    }, { /* 13 */
        {0, 0, 0, 0}, {0, 0, 0, 0}
    }, { /* 14 */
        {0, 0, 0, 0}, {0, 0, 0, 0}
    }, { /* 15 */
        {0, 0, 0, 0}, {0, 0, 0, 0}
    }, { /* 16 */
        {HANDS_PATTERN_16_2, HAND_PRIMAL_TRIO, HAND_KICKER_SOLO, HAND_CHAIN},
        {HANDS_PATTERN_16_4, HAND_PRIMAL_FOUR, HAND_KICKER_DUAL_PAIR, HAND_CHAIN}
    }, { /* 17 */
        {0, 0, 0, 0}, {0, 0, 0, 0}
    }, { /* 18 */
        {HANDS_PATTERN_18_3, HAND_PRIMAL_FOUR, HAND_KICKER_DUAL_SOLO, HAND_CHAIN},
        {0, 0, 0, 0}
    }, { /* 19 */
        {0, 0, 0, 0}, {0, 0, 0, 0}
    }, { /* 20 */
        {HANDS_PATTERN_20_2, HAND_PRIMAL_TRIO, HAND_KICKER_SOLO, HAND_CHAIN},
        {0, 0, 0, 0}
    }
};

/*
 * ************************************************************
 * hand
 * ************************************************************
 */
void Hand_Clear(hand_t *hand) {
  CardArray_Clear(&hand->cards);
  hand->type = 0;
}

void Hand_Copy(hand_t *dst, hand_t *src) {
  dst->type = src->type;
  CardArray_Copy(&dst->cards, &src->cards);
}

/*
 * ************************************************************
 * parser
 * ************************************************************
 */

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

  for (i = 0; i < array->length; i++) count[CARD_RANK(array->cards[i])]++;

  if (sort != NULL) {
    memcpy(sort, count, sizeof(int) * CARD_RANK_END);
    _Hand_SortCount(sort);
  }
}

/* check if a sorted count array matches specific pattern */
int _Hand_PatternMatch(int *sorted, int pattern) {
  int ret = 1;

#if (NICE_AND_CLEAN == 1)
  int i = 0;

  for (i = 0; i < PATTERN_LENGTH; i++) {
    if (sorted[i] != _hand_pattern[pattern][i]) {
      ret = 0;
      break;
    }
  }
#else
  ret = memcmp(sorted, _hand_pattern[pattern], sizeof(int) * PATTERN_LENGTH) == 0 ? 1 : 0;
#endif /* if NICE_AND_CLEAN == 1 */

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

    if (num == d1) CardArray_PushBack(&hand->cards, card);
    else if (num == d2) CardArray_PushBack(&temp, card);

    if (hand->cards.length + temp.length >= length) {
      CardArray_Concat(&hand->cards, &temp);
      break;
    }
  }
}

int _Hand_CheckNuke(hand_t *hand, card_array_t *array, int *count, int *sorted) {
  int ret = 0;
  if ((CARD_RANK(array->cards[0]) == CARD_RANK_R) &&
      (CARD_RANK(array->cards[1]) == CARD_RANK_r)) {
    hand->type = Hand_Format(HAND_PRIMAL_NUKE, HAND_KICKER_NONE, HAND_CHAIN_NONE);
    CardArray_Copy(&hand->cards, array);
    ret = hand->type;
  }

  return ret;
}

int _Hand_CheckBomb(hand_t *hand, card_array_t *array, int *count, int *sorted) {
  int ret = 0;

  if (_Hand_PatternMatch(sorted, HANDS_PATTERN_4_1)) {
    /* bomb, 4 */
    CardArray_Copy(&hand->cards, array);
    hand->type = Hand_Format(HAND_PRIMAL_BOMB, HAND_KICKER_NONE, HAND_CHAIN_NONE);
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
    if ((array->length < HAND_MIN_LENGTH) || (array->length > HAND_MAX_LENGTH)) {
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
          _Hand_Distribute(hand, array, count, d1, d2[kicker >> 4], array->length);
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

  DBGLog("Hand type: [");

  if (hand == NULL) {
    DBGLog("null\n");
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

  DBGLog("%s", toprint);

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

  if (toprint != NULL) DBGLog("%s", toprint);

  if (Hand_GetChain(hand->type) == HAND_CHAIN) DBGLog(" chain");

  DBGLog("]\n");

  CardArray_Print(&hand->cards);
}

/*
 * ************************************************************
 * hand list
 * ************************************************************
 */
void HandList_PushFront(rk_list_t **hl, hand_t *hand) {
  rk_list_t *node = rk_list_new();

  node->payload = (hand_t *) malloc(sizeof(hand_t));
  Hand_Copy((hand_t *) node->payload, hand);

  rk_list_push_front(hl, node);
}

void HandList_Remove(rk_list_t **hl, rk_list_t *node) {
  if (rk_list_remove(hl, node)) {
    free(node->payload);
    free(node);
  }
}

int _HandList_FindFunc(void *payload, void *context) {
  return ((hand_t *) payload)->type == *(int *) context ? 1 : 0;
}

rk_list_t *HandList_Find(rk_list_t **hl, int handtype) {
  return rk_list_search(*hl, &handtype, _HandList_FindFunc);
}

void HandList_Destroy(rk_list_t **hl) {
  rk_list_delete(hl, rk_algo_def_free);
}

hand_t *HandList_GetHand(rk_list_t *node) {
  return (hand_t *) node->payload;
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
      Hand_Format(HAND_PRIMAL_NUKE, HAND_KICKER_NONE, HAND_CHAIN_NONE))
    return 0;

  /* search for a higher rank bomb */
  if (tobeat->type == HAND_PRIMAL_BOMB) {
    canbeat = _HandList_SearchBeat_Primal(ctx, tobeat, beat, 4);
  }
  else {
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
          Hand_Format(HAND_PRIMAL_NUKE, HAND_KICKER_NONE, HAND_CHAIN_NONE);
      CardArray_CopyRank(&beat->cards, cards, CARD_RANK_R);
      CardArray_CopyRank(&beat->cards, cards, CARD_RANK_r);
    }
  }
  else {
    beat->type = Hand_Format(HAND_PRIMAL_BOMB, HAND_KICKER_NONE, HAND_CHAIN_NONE);
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

  /* same rank trio , case b*/
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
  int i = 0;
  int j = 0;
  int k = 0;
  int chainlength = 0;
  int found = 0;
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
      k = duplicate; /* how many cards needed for each rank */

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
  int i = 0;
  int j = 0;
  int chainlength = 0;
  int cantriobeat = 0;
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

    for (i = 0; i < htrio.cards.length;
         i += 3)
      kickcount[CARD_RANK(htrio.cards.cards[i])] = 0;

    /* remove count < kc and calculate n */
    for (i = CARD_RANK_3; i < CARD_RANK_END; i++) {
      if (kickcount[i] < kc) kickcount[i] = 0;
      else n++;
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

    for (i = 0; i < hkick.cards.length;
         i += kc)
      comb[j++] = rankcombmap[CARD_RANK(hkick.cards.cards[i])];

    /* find next combination */
    if (rk_next_comb(comb, chainlength, n)) {
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
    case Hand_Format(HAND_PRIMAL_SOLO, HAND_KICKER_NONE, HAND_CHAIN_NONE):
      canbeat = _HandList_SearchBeat_Primal(&ctx, tobeat, beat, HAND_PRIMAL_SOLO);
      break;

    case Hand_Format(HAND_PRIMAL_PAIR, HAND_KICKER_NONE, HAND_CHAIN_NONE):
      canbeat = _HandList_SearchBeat_Primal(&ctx, tobeat, beat, HAND_PRIMAL_PAIR);
      break;

    case Hand_Format(HAND_PRIMAL_TRIO, HAND_KICKER_NONE, HAND_CHAIN_NONE):
      canbeat = _HandList_SearchBeat_Primal(&ctx, tobeat, beat, HAND_PRIMAL_TRIO);
      break;

    case Hand_Format(HAND_PRIMAL_TRIO, HAND_KICKER_PAIR, HAND_CHAIN_NONE):
      canbeat =
          _HandList_SearchBeat_TrioKicker(&ctx, tobeat, beat, HAND_PRIMAL_PAIR);
      break;

    case Hand_Format(HAND_PRIMAL_TRIO, HAND_KICKER_SOLO, HAND_CHAIN_NONE):
      canbeat =
          _HandList_SearchBeat_TrioKicker(&ctx, tobeat, beat, HAND_PRIMAL_SOLO);
      break;

    case Hand_Format(HAND_PRIMAL_SOLO, HAND_KICKER_NONE, HAND_CHAIN):
      canbeat = _HandList_SearchBeat_Chain(&ctx, tobeat, beat, HAND_PRIMAL_SOLO);
      break;

    case Hand_Format(HAND_PRIMAL_PAIR, HAND_KICKER_NONE, HAND_CHAIN):
      canbeat = _HandList_SearchBeat_Chain(&ctx, tobeat, beat, HAND_PRIMAL_PAIR);
      break;

    case Hand_Format(HAND_PRIMAL_TRIO, HAND_KICKER_NONE, HAND_CHAIN):
      canbeat = _HandList_SearchBeat_Chain(&ctx, tobeat, beat, HAND_PRIMAL_TRIO);
      break;

    case Hand_Format(HAND_PRIMAL_FOUR, HAND_KICKER_NONE, HAND_CHAIN):
      canbeat = _HandList_SearchBeat_Chain(&ctx, tobeat, beat, HAND_PRIMAL_FOUR);
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
  if (beat->type != 0) return _HandList_SearchBeat(cards, beat, beat);
  else return _HandList_SearchBeat(cards, tobeat, beat);
}

rk_list_t *HandList_SearchBeatList(card_array_t *cards, hand_t *tobeat) {
  rk_list_t *hl = NULL;
  hand_t htobeat;
  hand_t beat;
  int canbeat = 0;

  Hand_Clear(&beat);
  Hand_Copy(&htobeat, tobeat);

  do {
    canbeat = _HandList_SearchBeat(cards, &htobeat, &beat);

    if (canbeat) {
      Hand_Copy(&htobeat, &beat);
      HandList_PushFront(&hl, &beat);
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
void _HandList_ExtractConsecutive(rk_list_t **hl,
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

  cardnum = array->length / duplicate;
  lastrank = CARD_RANK(array->cards[0]);
  i = duplicate;

  while (cardnum--) {
    if ((lastrank - 1) != CARD_RANK(array->cards[i])) {
      /* chain break */
      if (i >= chainlen[duplicate]) {
        /* chain */
        Hand_Clear(&hand);
        hand.type = Hand_Format(primal[duplicate], HAND_KICKER_NONE, HAND_CHAIN);

        for (j = 0; j < i; j++)
          CardArray_PushBack(&hand.cards,
                             CardArray_PopFront(array));

        HandList_PushFront(hl, &hand);
      } else {
        /* not a chain */
        for (j = 0; j < i / duplicate; j++) {
          Hand_Clear(&hand);
          hand.type = Hand_Format(primal[duplicate],
                                  HAND_KICKER_NONE,
                                  HAND_CHAIN_NONE);

          for (k = 0; k < duplicate; k++)
            CardArray_PushBack(&hand.cards,
                               CardArray_PopFront(
                                   array));

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

  k = i - duplicate; /* step back */

  /* all chained up */
  if ((k != 0) && (k == array->length)) {
    /* can chain up */
    if (k >= chainlen[duplicate]) {
      Hand_Clear(&hand);
      hand.type = Hand_Format(primal[duplicate], HAND_KICKER_NONE, HAND_CHAIN);

      for (j = 0; j < i - duplicate; j++)
        CardArray_PushBack(&hand.cards,
                           CardArray_PopFront(
                               array));

      HandList_PushFront(hl, &hand);
    } else {
      for (j = 0; j < k / duplicate; j++) {
        Hand_Clear(&hand);
        hand.type = Hand_Format(primal[duplicate],
                                HAND_KICKER_NONE,
                                HAND_CHAIN_NONE);

        for (i = 0; i < duplicate; i++)
          CardArray_PushBack(&hand.cards,
                             CardArray_PopFront(
                                 array));

        HandList_PushFront(hl, &hand);
      }
    }
  }
}

void _HandList_ExtractNukeBomb2(rk_list_t **hl, card_array_t *array, int *count) {
  int i = 0;
  hand_t hand;

  /* nuke */
  if (count[CARD_RANK_r] && count[CARD_RANK_R]) {
    Hand_Clear(&hand);
    hand.type = Hand_Format(HAND_PRIMAL_NUKE, HAND_KICKER_NONE, HAND_CHAIN_NONE);
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
          Hand_Format(HAND_PRIMAL_BOMB, HAND_KICKER_NONE, HAND_CHAIN_NONE);
      CardArray_CopyRank(&hand.cards, array, i);

      HandList_PushFront(hl, &hand);

      count[i] = 0;
      CardArray_RemoveRank(array, i);
    }
  }

  /* joker */
  if ((count[CARD_RANK_r] != 0) || (count[CARD_RANK_R] != 0)) {
    Hand_Clear(&hand);
    CardArray_CopyRank(&hand.cards,
                       array,
                       count[CARD_RANK_r] != 0 ? CARD_RANK_r : CARD_RANK_R);
    hand.type = Hand_Format(HAND_PRIMAL_SOLO, HAND_KICKER_NONE, HAND_CHAIN_NONE);

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
            Hand_Format(HAND_PRIMAL_SOLO, HAND_KICKER_NONE, HAND_CHAIN_NONE);
        break;

      case 2:
        hand.type =
            Hand_Format(HAND_PRIMAL_PAIR, HAND_KICKER_NONE, HAND_CHAIN_NONE);
        break;

      case 3:
        hand.type =
            Hand_Format(HAND_PRIMAL_TRIO, HAND_KICKER_NONE, HAND_CHAIN_NONE);
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

  /* nuke, bomb and 2 */
  _HandList_ExtractNukeBomb2(&hl, &array, count);

  /* chains */
  for (i = 0; i < array.length;) {
    int c = count[CARD_RANK(array.cards[i])];

    if (c != 0) {
      CardArray_PushBackCards(arrprimals[c - 1], &array, i, c);
      i += c;
    }
    else {
      i++;
    }
  }

  /* chain */
  _HandList_ExtractConsecutive(&hl, &arrtrio, 3);
  _HandList_ExtractConsecutive(&hl, &arrpair, 2);
  _HandList_ExtractConsecutive(&hl, &arrsolo, 1);

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
        for (j = 0; j < i; j++) CardArray_PopFront(array);

        hands++;
      }
      else {
        /* not a chain */
        for (j = 0; j < i / duplicate; j++) {
          for (k = 0; k < duplicate; k++) CardArray_PopFront(array);

          hands++;
        }
      }

      if (array->length == 0) break;

      lastrank = CARD_RANK(array->cards[0]);
      i = duplicate;
    }
    else /* chain intact */
    {
      lastrank = CARD_RANK(array->cards[i]);
      i += duplicate;
    }
  }

  k = i - duplicate;                    /* step back */

  if ((k != 0) && (k == array->length)) /* all chained up */
  {
    /* can chain up */
    if (k >= chainlen[duplicate]) {
      for (j = 0; j < i - duplicate; j++) CardArray_PopFront(array);

      hands++;
    }
    else {
      for (j = 0; j < k / duplicate; j++) {
        for (i = 0; i < duplicate; i++) CardArray_PopFront(array);

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
    }
    else {
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
  lastrank = CARD_RANK(cards->cards[0]);

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
          Hand_Format(primals[primal], HAND_KICKER_NONE, HAND_CHAIN_NONE);
      break;
    }
  }
}

typedef void (*_HandList_SearchPrimalFunc)(hand_ctx_t *,
                                           hand_t *,
                                           int);

#define HAND_SEARCH_TYPES           3

/*
 * pass a empty hand to start traverse
 * result stores in hand
 * return 0 when stop
 */
int _HLAA_TraverseHands(hand_ctx_t *ctx, int *begin, hand_t *hand) {
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
      }
      else {
        i++;
        *begin = i;
      }
    }

    /* if found == 0, should PANIC */
  }

    /* continue search via beat */
  else {
    found = HandList_SearchBeat(&ctx->cards, hand, hand);
  }

  return found;
}

/*
 * extract all chains or primal hands in hand_ctx
 */
void _HLAA_ExtractAllChains(hand_ctx_t *ctx, rk_list_t **hands) {
  int found = 0;
  int lastsearch = 0;
  hand_t workinghand;
  hand_t lasthand;

  /* init search */
  Hand_Clear(&workinghand);
  Hand_Clear(&lasthand);

  found = _HLAA_TraverseHands(ctx, &lastsearch, &lasthand);

  while (found != 0) {
    HandList_PushFront(hands, &lasthand);

    Hand_Copy(&workinghand, &lasthand);

    while ((found =
                _HLAA_TraverseHands(ctx, &lastsearch,
                                    &workinghand)) != 0)
      HandList_PushFront(
          hands,
          &workinghand);

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
        found = _HLAA_TraverseHands(ctx, &lastsearch, &lasthand);
      }
    }
  }
}

/* advanced search tree payload */
typedef struct _hltree_payload_s {
  hand_ctx_t ctx;
  /* hand context */
  hand_t hand;
  /* hand */
  int weight; /* evaluation weight */
} _hltree_payload_t;

rk_tree_t *_HLAA_TreeAddHand(rk_tree_t **tree, rk_list_t *hand) {
  rk_tree_t *node = NULL;
  _hltree_payload_t *oldpayload = NULL;
  _hltree_payload_t *newpayload = NULL;

  oldpayload = (_hltree_payload_t *) (*tree)->payload;
  newpayload = (_hltree_payload_t *) malloc(sizeof(_hltree_payload_t));

  memcpy(&newpayload->ctx, &oldpayload->ctx, sizeof(hand_ctx_t));
  Hand_Copy(&newpayload->hand, (hand_t *) hand->payload);
  CardArray_Subtract(&newpayload->ctx.cards, &HandList_GetHand(hand)->cards);
  CardArray_Copy(&newpayload->ctx.rcards, &newpayload->ctx.cards);
  CardArray_Reverse(&newpayload->ctx.rcards);
  Hand_CountRank(&newpayload->ctx.cards, newpayload->ctx.count, NULL);
  newpayload->weight = oldpayload->weight + 1;

  node = (rk_tree_t *) calloc(1, sizeof(rk_tree_t));
  node->payload = newpayload;
  rk_tree_add_child(tree, node);

  return node;
}

void _HLAA_StackPushTreeNode(rk_stack_t **st, rk_tree_t *tree) {
  rk_stack_t *node = (rk_stack_t *) malloc(sizeof(rk_stack_t));

  node->payload = tree;
  rk_stack_push(st, node);
}

/*
 * search hand via least hands
 */
rk_list_t *HandList_AdvancedAnalyze(card_array_t *array) {
  rk_list_t *handlist = NULL;
  rk_list_t *chains = NULL;
  rk_list_t *others = NULL;
  rk_list_t *hlnode = NULL;
  rk_stack_t *st = NULL;
  rk_stack_t *temp = NULL;
  rk_tree_t *grandtree = NULL;
  rk_tree_t *workingtree = NULL;
  rk_tree_t *tnode = NULL;
  rk_tree_t *shortest = NULL;
  _hltree_payload_t *pload = NULL;

  hand_ctx_t ctx;

  /* setup search context */
  HandCtx_Clear(&ctx);

  /* build beat search context */
  Hand_CountRank(array, ctx.count, NULL);
  CardArray_Copy(&ctx.cards, array);

  /* extract bombs and 2 */
  _HandList_ExtractNukeBomb2(&handlist, &ctx.cards, ctx.count);

  /* finish building beat_search_context */
  CardArray_Copy(&ctx.rcards, &ctx.cards);
  CardArray_Reverse(&ctx.rcards);

  /* magic goes here */

  /* root */
  pload = (_hltree_payload_t *) malloc(sizeof(_hltree_payload_t));
  memcpy(&pload->ctx, &ctx, sizeof(hand_ctx_t));
  pload->weight = 0;

  grandtree = (rk_tree_t *) calloc(1, sizeof(rk_tree_t));
  grandtree->payload = pload;

  /* first expansion */
  _HLAA_ExtractAllChains(&ctx, &chains);

  /* no chains, fall back to standard analyze */
  if (chains == NULL) {
    HandList_Destroy(&handlist);
    rk_tree_delete(&grandtree, rk_algo_def_free);
    return HandList_StandardAnalyze(array);
  }

  hlnode = chains;

  while (hlnode != NULL) {
    tnode = _HLAA_TreeAddHand(&grandtree, hlnode);
    _HLAA_StackPushTreeNode(&st, tnode);

    hlnode = hlnode->next;
  }

  HandList_Destroy(&chains);

  /* loop start */
  while (st != NULL) {
    /* pop stack */
    temp = rk_stack_pop(&st);

    /* extract node data */
    workingtree = (rk_tree_t *) temp->payload;
    free(temp);
    chains = NULL;
    pload = (_hltree_payload_t *) workingtree->payload;

    /* expansion */
    _HLAA_ExtractAllChains(&pload->ctx, &chains);

    if (chains != NULL) {
      /* push new nodes */
      hlnode = chains;

      while (hlnode != NULL) {
        tnode = _HLAA_TreeAddHand(&workingtree, hlnode);
        _HLAA_StackPushTreeNode(&st, tnode);

        hlnode = hlnode->next;
      }

      HandList_Destroy(&chains);
      chains = NULL;
    }
  }

  /* tree construction complete */
  st = NULL;
  rk_tree_dump_leaf(grandtree, &st);

  /* find shortest path */
  while (st != NULL) {
    /* pop stack */
    temp = rk_stack_pop(&st);

    /* extract node data */
    workingtree = (rk_tree_t *) temp->payload;
    free(temp);
    pload = (_hltree_payload_t *) workingtree->payload;

    /* calculate other hands weight */
    pload->weight += HandList_StandardEvaluator(&pload->ctx.cards);

    if ((shortest == NULL) ||
        (pload->weight < ((_hltree_payload_t *) shortest->payload)->weight))
      shortest = workingtree;
  }

  /* extract shortest node's other hands */
  HandList_Destroy(&others);
  others = HandList_StandardAnalyze(&((_hltree_payload_t *) (shortest->payload))->ctx.cards);

  while (shortest != NULL &&
      ((_hltree_payload_t *) shortest->payload)->weight != 0) {
    HandList_PushFront(&others, &((_hltree_payload_t *) (shortest->payload))->hand);
    shortest = shortest->root;
  }

  rk_list_concat(&others, handlist);

  rk_tree_delete(&grandtree, rk_algo_def_free);

  return others;
}

int HandList_AdvancedEvaluator(card_array_t *array) {
  int value = 0;
  rk_list_t *hl = HandList_AdvancedAnalyze(array);

  value = rk_list_len(hl);
  HandList_Destroy(&hl);

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
  rk_list_t *node;
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
  rk_list_t *node = NULL;
  card_array_t temp;
  beat_node_t *hnodes[BEAT_NODE_CAPACITY];
  rk_list_t *hbombs[BEAT_NODE_CAPACITY];
  HandList_EvaluateFunc evalFunc;

  evalFunc = (func == NULL) ? HandList_StandardEvaluator : func;

  memset(hnodes, 0, sizeof(beat_node_t *) * BEAT_NODE_CAPACITY);
  memset(hbombs, 0, sizeof(rk_list_t *) * BEAT_NODE_CAPACITY);

  /* search beat list */
  hl = HandList_SearchBeatList(array, tobeat);

  /* separate bomb/nuke and normal hands */
  node = hl;

  while (node != NULL) {
    if ((HandList_GetHand(node)->type ==
        Hand_Format(HAND_PRIMAL_BOMB, HAND_KICKER_NONE, HAND_CHAIN_NONE)) ||
        (HandList_GetHand(node)->type ==
            Hand_Format(HAND_PRIMAL_NUKE, HAND_KICKER_NONE, HAND_CHAIN_NONE))) {
      hbombs[bombi++] = node;
    }
    else {
      hnodes[nodei] = (beat_node_t *) malloc(sizeof(beat_node_t));
      hnodes[nodei]->node = node;
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
      leftover = HandList_GetHand(hnodes[i]->node);
      CardArray_Subtract(&temp, &leftover->cards);

      hnodes[i]->value = evalFunc(&temp) *
          BEAT_VALUE_FACTOR +
          CARD_RANK(leftover->cards.cards[0]);
    }

    /* sort primal hands */
    qsort(hnodes, nodei, sizeof(beat_node_t *), _BeatNode_ValueSort);
  }

  /* re-build hand list */
  hl = NULL;

  for (i = bombi; i >= 0; i--) {
    if (hbombs[i]) {
      hbombs[i]->next = NULL;
      rk_list_push_front(&hl, hbombs[i]);
    }
  }

  for (i = nodei; i >= 0; i--) {
    if (hnodes[i]) {
      hnodes[i]->node->next = NULL;
      rk_list_push_front(&hl, hnodes[i]->node);
    }
  }

  /* select beat */
  if (hl != NULL) {
    Hand_Copy(beat, HandList_GetHand(hl));
    canbeat = 1;
  }

  /* clean up */
  for (i = 0; i < nodei; i++) free(hnodes[i]);

  HandList_Destroy(&hl);

  return canbeat;
}

void HandList_Print(rk_list_t *hl) {
  rk_list_t *node = hl;

  if ((node == NULL) || (HandList_GetHand(node) == NULL) ||
      (HandList_GetHand(node)->type == 0))
    return;

  DBGLog("-----hand_list_t begin---------\n");

  for (node = hl; node != NULL; node = node->next) {
    Hand_Print(HandList_GetHand(node));
  }
  DBGLog("-----hand_list_t ended---------\n");
}
