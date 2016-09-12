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

#include "card.h"

const int32_t _card_prime[] = {
  0,
  CARD_PRIME_3, CARD_PRIME_4, CARD_PRIME_5, CARD_PRIME_6,
  CARD_PRIME_7, CARD_PRIME_8, CARD_PRIME_9, CARD_PRIME_T,
  CARD_PRIME_J, CARD_PRIME_Q, CARD_PRIME_K, CARD_PRIME_A,
  CARD_PRIME_2, CARD_PRIME_r, CARD_PRIME_R
};

const int32_t _card_bin[] = {
  0,
  CARD_BIN_3, CARD_BIN_4, CARD_BIN_5, CARD_BIN_6,
  CARD_BIN_7, CARD_BIN_8, CARD_BIN_9, CARD_BIN_T,
  CARD_BIN_J, CARD_BIN_Q, CARD_BIN_K, CARD_BIN_A,
  CARD_BIN_2, CARD_BIN_r, CARD_BIN_R
};

const int32_t _card_preset[] = {
  0x00011102, 0x00012102, 0x00014102, 0x00018102,
  0x00021203, 0x00022203, 0x00024203, 0x00028203,
  0x00041305, 0x00042305, 0x00044305, 0x00048305,
  0x00081407, 0x00082407, 0x00084407, 0x00088407,
  0x0010150B, 0x0010250B, 0x0010450B, 0x0010850B,
  0x0020160D, 0x0020260D, 0x0020460D, 0x0020860D,
  0x00401711, 0x00402711, 0x00404711, 0x00408711,
  0x00801813, 0x00802813, 0x00804813, 0x00808813,
  0x01001917, 0x01002917, 0x01004917, 0x01008917,
  0x02001A1D, 0x02002A1D, 0x02004A1D, 0x02008A1D,
  0x04001B1F, 0x04002B1F, 0x04004B1F, 0x04008B1F,
  0x08001C25, 0x08002C25, 0x08004C25, 0x08008C25,
  0x10001D29, 0x10002D29, 0x10004D29, 0x10008D29,
  0x20001E2B, 0x40002F2F
};

int32_t Card_Make(int32_t suit, int32_t rank) {
  int32_t card = 0;

  card |= _card_prime[rank] << CARD_BIT_OFFSET_PRIME;
  card |= rank << CARD_BIT_OFFSET_RANK;
  card |= suit << CARD_BIT_OFFSET_SUIT;
  card |= _card_bin[rank] << CARD_BIT_OFFSET_BIN;

  return card;
}

/* card set generator
int main(int argc, char* argv[]) {
  for (int rank = CARD_RANK_3; rank < CARD_RANK_r; rank++) {
    for (int suit = 1; suit <= 0x08; suit *= 2) {
      if (suit == 1) {
        printf("\n");
      }
      int32_t c = Card_Make(suit, rank);
      printf("0x%08X, ", c);
    }
  }

  printf("\n0x%08X, ", Card_Make(CARD_SUIT_SPADE, CARD_RANK_r));
  printf("0x%08X", Card_Make(CARD_SUIT_HEART, CARD_RANK_R));


  return 0;
}
 */

void *CardArray_InitFromString(card_array_t *array, const char *str) {
  int32_t rank = 0;
  int32_t suit = 0;
  const char *p = str;

  CardArray_Clear(array);

  while (*p != '\0') {
    switch ((uint8_t) *p) {
#if LL_GRAPHICAL_SUIT != 0
      case 0xA6:
        suit = CARD_SUIT_DIAMOND;
        break;

      case 0xA3:
        suit = CARD_SUIT_CLUB;
        break;

      case 0xA5:
        suit = CARD_SUIT_HEART;
        break;

      case 0xA0:
        suit = CARD_SUIT_SPADE;
        break;

#else /* ifdef LL_GRAPHICAL_SUIT */
      case 'd':
        suit = CARD_SUIT_DIAMOND;
        break;

      case 'c':
        suit = CARD_SUIT_CLUB;
        break;

      case 'h':
        suit = CARD_SUIT_HEART;
        break;

      case 's':
        suit = CARD_SUIT_SPADE;
        break;

#endif /* ifdef LL_GRAPHICAL_SUIT */
      case 'T':
        rank = CARD_RANK_T;
        break;

      case 'J':
        rank = CARD_RANK_J;
        break;

      case 'Q':
        rank = CARD_RANK_Q;
        break;

      case 'K':
        rank = CARD_RANK_K;
        break;

      case 'A':
        rank = CARD_RANK_A;
        break;

      case '2':
        rank = CARD_RANK_2;
        break;

      case 'r':
        rank = CARD_RANK_r;
        break;

      case 'R':
        rank = CARD_RANK_R;
        break;

      default:
        break;
    }

    if ((*p <= '9') && (*p >= '3'))
      rank = CARD_RANK_3 + (int32_t) ((*p) - '3');

    if (suit != 0 && rank != 0) {
      CardArray_PushBack(array, Card_Make(suit, rank));
      suit = 0;
      rank = 0;
    }

    p++;
  }

  return array;
}

void CardArray_Reset(card_array_t *array) {
  CardsCopy(array->cards, _card_preset, CARD_SET_LENGTH);
  array->length = CARD_SET_LENGTH;
}

int CardArray_Concat(card_array_t *head, card_array_t *tail) {
  int length = 0;
  int slot = 0;

  slot = CARD_SET_LENGTH - head->length;

  if (!CardArray_IsEmpty(tail) && (slot > 0)) {
    length = slot >= tail->length ? tail->length : slot;
    CardsCopy(&head->cards[head->length], tail->cards, length);
    head->length += length;
  } else {
    length = 0;
  }

  return length;
}

void CardArray_Subtract(card_array_t *from, card_array_t *sub) {
  int i = 0;
  int j = 0;
  int32_t card = 0;
  card_array_t temp;

  CardArray_Clear(&temp);

  for (i = 0; i < from->length; i++) {
    card = from->cards[i];

    for (j = 0; j < sub->length; j++) {
      if (card == sub->cards[j]) {
        card = 0;
        break;
      }
    }

    if (card != 0) CardArray_PushBack(&temp, card);
  }

  CardArray_Copy(from, &temp);
}

int CardArray_IsIdentity(card_array_t *a, card_array_t *b) {
  int i = 0;
  int identity = 1;
  card_array_t ta, tb;

  if (a == b) return 1;

  identity = a->length - b->length;

  if (identity == 0) {
    CardArray_Copy(&ta, a);
    CardArray_Copy(&tb, b);
    CardArray_Sort(&ta, NULL);
    CardArray_Sort(&tb, NULL);

    for (i = 0; i < a->length; i++) {
      if (ta.cards[i] != tb.cards[i]) {
        identity = 0;
        break;
      }
    }
  }

  return identity;
}

int CardArray_IsContain(card_array_t *array, card_array_t *segment) {
  int contain = 0;
  int i = 0;
  int j = 0;
  card_array_t temp;

  if ((array->length == 0) || (segment->length == 0)) return contain;

  CardArray_Copy(&temp, segment);

  if (array->length >= segment->length) {
    for (i = 0; i < array->length; i++) {
      for (j = 0; j < temp.length; j++) {
        if (array->cards[i] == temp.cards[j]) {
          CardArray_RemoveCard(&temp, temp.cards[j]);
          break;
        }
      }
    }

    contain = temp.length == 0 ? 1 : 0;
  }

  return contain;
}

void CardArray_PushBack(card_array_t *array, int32_t card) {
  if (!CardArray_IsFull(array)) array->cards[array->length++] = card;
}

int32_t CardArray_PushFront(card_array_t *array, int32_t card) {
  int32_t ret = 0;

  if (!CardArray_IsFull(array)) {
    CardsMove(array->cards + 1, array->cards, array->length);
    array->cards[0] = card;

    array->length++;

    ret = card;
  }

  return ret;
}

int32_t CardArray_PopFront(card_array_t *array) {
  int32_t card = 0;

  if (!CardArray_IsEmpty(array)) {
    card = array->cards[0];
    CardsMove(array->cards, array->cards + 1, array->length);
    array->cards[array->length--] = 0;
  }

  return card;
}

int32_t CardArray_PopBack(card_array_t *array) {
  int32_t card = 0;

  if (!CardArray_IsEmpty(array)) {
    card = array->cards[array->length - 1];
    array->cards[array->length - 1] = 0;
    array->length--;
  }

  return card;
}

int CardArray_DropFront(card_array_t *array, int count) {
  int drop = 0;

  drop = (array->length >= count) ? count : array->length;
  CardsMove(array->cards, array->cards + drop, array->length);
  array->length -= drop;
  memset(array->cards + array->length, 0, sizeof(int32_t) * drop);

  return drop;
}

int CardArray_DropBack(card_array_t *array, int count) {
  int drop = 0;

  drop = (array->length >= count) ? count : array->length;
  memset(array->cards + array->length - drop, 0, sizeof(int32_t) * drop);
  array->length -= drop;

  return drop;
}

void CardArray_Insert(card_array_t *array, int before, int32_t card) {
  if (!CardArray_IsFull(array)) {
    if (before == 0) {
      CardArray_PushFront(array, card);
    } else if (before == array->length) {
      CardArray_PushBack(array, card);
    } else if ((before > 0) && (before < array->length)) {
      CardsMove(array->cards + before + 1,
              array->cards + before,
              array->length - before);
      array->cards[before] = card;
      array->length++;
    }
  }
}

int32_t CardArray_Remove(card_array_t *array, int where) {
  int32_t ret = 0;

  if (!CardArray_IsEmpty(array)) {
    if (where == 0) {
      ret = CardArray_PopFront(array);
    } else if (where == array->length - 1) {
      ret = CardArray_PopBack(array);
    } else if ((where > 0) && (where < array->length - 1)) {
      ret = array->cards[where];
      array->length--;
      CardsMove(array->cards + where,
              array->cards + where + 1,
              array->length - where);
      array->cards[array->length] = 0;
    }
  }

  return ret;
}

int32_t CardArray_RemoveCard(card_array_t *array, int32_t card) {
  int32_t ret = 0;
  int i = 0;

  for (i = 0; i < array->length; i++) {
    if (array->cards[i] == card) {
      ret = card;
      CardArray_Remove(array, i);
      break;
    }
  }

  return ret;
}

int CardArray_PushBackCards(card_array_t *array,
                            card_array_t *from,
                            int where,
                            int count) {
  int cards = 0;
  int i = 0;

  for (i = 0; i < count; i++) {
    CardArray_PushBack(array, from->cards[where + i]);
    cards++;
  }

  return cards;
}

int _CA_DescentSort(const void *a, const void *b) {
  return *(int *) b - *(int *) a;
}

void CardArray_CountRank(card_array_t *array, int *count, int *sort) {
  int i = 0;
  memset(count, 0, sizeof(int) * CARD_RANK_END);

  for (i = 0; i < array->length; i++)
    count[CARD_RANK(array->cards[i])]++;

  if (sort != NULL) {
    memcpy(sort, count, sizeof(int) * CARD_RANK_END);
    qsort(sort, CARD_RANK_END, sizeof(int), _CA_DescentSort);
  }
}

void CardArray_CopyRank(card_array_t *dst, card_array_t *src, int32_t rank) {
  int i = 0;

  for (i = 0; i < src->length; i++) {
    if (CARD_RANK(src->cards[i]) == rank)
      CardArray_PushBack(dst, src->cards[i]);
  }
}

void CardArray_RemoveRank(card_array_t *array, int32_t rank) {
  int i = 0;

  card_array_t temp;

  CardArray_Clear(&temp);

  for (i = 0; i < array->length; i++) {
    if (CARD_RANK(array->cards[i]) != rank)
      CardArray_PushBack(&temp, array->cards[i]);
  }

  CardArray_Copy(array, &temp);
}

int CardArray_StandardSort(const void *a, const void *b) {
  return (*(int32_t *) b) - (*(int32_t *) a);
}

void CardArray_Sort(card_array_t *array, int (*comparator)(const void *,
                                                           const void *)) {
  if (comparator == NULL)
    qsort(array->cards,
          (size_t) array->length,
          sizeof(int32_t),
          CardArray_StandardSort);
  else
    qsort(array->cards,
          (size_t) array->length,
          sizeof(int32_t),
          comparator);
}

void CardArray_Reverse(card_array_t *array) {
  int i = 0;
  card_array_t temp;

  CardArray_Clear(&temp);

  for (i = array->length; i--;) temp.cards[temp.length++] = array->cards[i];

  CardArray_Copy(array, &temp);
}

#if LL_GRAPHICAL_SUIT != 0
# define CARD_STRING_LENGTH 4

unsigned char szDIAMOND[] = {0xE2, 0x99, 0xA6, 0};
unsigned char szCLUB[] = {0xE2, 0x99, 0xA3, 0};
unsigned char szHEART[] = {0xE2, 0x99, 0xA5, 0};
unsigned char szSPADE[] = {0xE2, 0x99, 0xA0, 0};
#else /* ifdef LL_GRAPHICAL_SUIT */
# define CARD_STRING_LENGTH 2

unsigned char szDIAMOND[] = {'d', 0, 0, 0};
unsigned char szCLUB[] = {'c', 0, 0, 0};
unsigned char szHEART[] = {'h', 0, 0, 0};
unsigned char szSPADE[] = {'s', 0, 0, 0};
#endif /* ifdef LL_GRAPHICAL_SUIT */

char szRank[] =
  {'3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A', '2', 'r', 'R'};

int Card_ToString(int32_t card, char *buf, int len) {
  if ((buf != NULL) && (len >= CARD_STRING_LENGTH)) {
    char *szSuit = NULL;
    int rank = CARD_RANK(card);
    int suit = CARD_SUIT(card);

    switch (suit) {
      case CARD_SUIT_DIAMOND:
        szSuit = (char *) szDIAMOND;
        break;

      case CARD_SUIT_CLUB:
        szSuit = (char *) szCLUB;
        break;

      case CARD_SUIT_HEART:
        szSuit = (char *) szHEART;
        break;

      case CARD_SUIT_SPADE:
        szSuit = (char *) szSPADE;
        break;

      default:
        szSuit = "ERR";
        break;
    }
    memcpy(buf, szSuit, CARD_STRING_LENGTH - 1);
    buf[CARD_STRING_LENGTH - 1] = szRank[rank - CARD_RANK_3];
  }

  return CARD_STRING_LENGTH;
}

void CardArray_Print(card_array_t *array) {
  int i = 0;
  char str[10];

  memset(str, 0, 10);
  DBGLog("Cards: (%d): ", array->length);

  for (i = 0; i < array->length; i++) {
    Card_ToString(array->cards[i], str, 10);
    DBGLog("%s ", str);
  }

  DBGLog("\n");
}
