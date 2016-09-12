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

#include "standard_ai.h"
#include "game.h"

int StandardAI_GetReady(void *p, void *game) {
  player_t *player = (player_t *) p;

  CardArray_Sort(&player->cards, NULL);
  CardArray_Copy(&player->record, &player->cards);
#if (PRINT_GAME_LOG == 1)
  CardArray_Print(&player->record);
#endif /* ifdef PRINT_GAME_LOG */
  // FIXME
  player->handlist = HandList_StandardAnalyze(&player->cards);

  HandList_Print(player->handlist);

  return 0;
}

int StandardAI_Bid(void *p, void *g) {
  int shouldbid = 0;
  int handlistlen = 0;
  game_t *game = (game_t *) g;
  player_t *player = (player_t *) p;

  CardArray_Sort(&player->cards, NULL);
  player->handlist = HandList_StandardAnalyze(&player->cards);
  handlistlen = rk_list_count(player->handlist);
  rk_list_clear_destroy(player->handlist);

  if (handlistlen > 9) {
    shouldbid = 0;
  } else if ((handlistlen < 9) && (handlistlen > 3)) {
    shouldbid = 1;
  } else if ((handlistlen <= 3) && (handlistlen > 2)) {
    shouldbid = 2;
  } else if (handlistlen <= 2) {
    shouldbid = 3;
  }

  if (shouldbid > game->bid)
    return shouldbid;
  else
    return 0;
}

int StandardAI_Play(void *p, void *game) {
  int countpair = 0;
  int countsolo = 0;
  int need = 0;
  int searchprimal = 0;
  int kicker = 0;
  player_t *player = (player_t *) p;
  hand_t *node = NULL;
  rk_list_node_t *temp = NULL;

  hand_t *hand = &((game_t *) game)->lastHand;

  do {
    /* empty hands */
    if (player->handlist == NULL) {
      hand->type = 0;
      break;
    }

    /* last hand */
    if (rk_list_count(player->handlist) == 1) {
      Hand_Copy(hand, (hand_t *) player->handlist->first->payload);
      HandList_Remove(player->handlist, player->handlist->first->payload);
      break;
    }

    /* try to find longest hand combination */
    node = HandList_Find(player->handlist,
                         Hand_Format(HAND_PRIMAL_TRIO,
                                     HAND_KICKER_NONE,
                                     HAND_CHAIN));

    if (node != NULL) {
      /* copy hand*/
      Hand_Copy(hand, node);

      /* how many kickers do we need */
      need = node->cards.length / 3;

      /* remove hand node from hand list */
      HandList_Remove(player->handlist, node);

      /* count solo and pair number */
      temp = player->handlist->first;
      countpair = 0;
      countsolo = 0;

      while (temp->next != NULL) {
        if (HandList_GetHand(temp)->type == HAND_PRIMAL_PAIR)
          countpair++;
        else if (HandList_GetHand(temp)->type == HAND_PRIMAL_SOLO)
          countsolo++;

        temp = temp->next;
      }

      /* trio-pair-chain then trio-solo-chain */
      if ((countsolo < need) && (countpair < need)) {
        break;
      } else if (countpair >= need) {
        searchprimal = HAND_PRIMAL_PAIR;
        kicker = HAND_KICKER_PAIR;
      } else {
        searchprimal = HAND_PRIMAL_SOLO;
        kicker = HAND_KICKER_SOLO;
      }

      /* detach pairs from list */
      temp = player->handlist->first;

      while (need > 0 && temp != NULL) {
        if (HandList_GetHand(temp)->type ==
          Hand_Format(searchprimal, HAND_KICKER_NONE, HAND_CHAINLESS)) {
          /* copy cards */
          CardArray_Concat(&hand->cards, &HandList_GetHand(temp)->cards);
          HandList_Remove(player->handlist, temp->payload);
          temp = player->handlist->first;
          need--;
        } else {
          temp = temp->next;
        }
      }

      Hand_SetKicker(hand->type, kicker);
      break;
    }

    /* pair chain */
    node = HandList_Find(player->handlist,
                    Hand_Format(HAND_PRIMAL_PAIR,
                                HAND_KICKER_NONE,
                                HAND_CHAIN));

    /* solo chain */
    if (node == NULL)
      node = HandList_Find(player->handlist,
                           Hand_Format(HAND_PRIMAL_SOLO, HAND_KICKER_NONE,
                                       HAND_CHAIN));

    if (node != NULL) {
      Hand_Copy(hand, node);
      HandList_Remove(player->handlist, node);
      break;
    }

    /* trio */
    node = HandList_Find(player->handlist,
                         Hand_Format(HAND_PRIMAL_TRIO, HAND_KICKER_NONE,
                                     HAND_CHAINLESS));

    if ((node != NULL) &&
      (CARD_RANK(node->cards.cards[0]) != CARD_RANK_2)) {
      Hand_Copy(hand, node);
      HandList_Remove(player->handlist, node);

      /* pair */
      node = HandList_Find(player->handlist,
                           Hand_Format(HAND_PRIMAL_PAIR, HAND_KICKER_NONE,
                                       HAND_CHAINLESS));

      if ((node != NULL) &&
        (CARD_RANK(node->cards.cards[0]) != CARD_RANK_2)) {
        kicker = HAND_KICKER_PAIR;
      } else {
        /* solo */
        node = HandList_Find(player->handlist,
                             Hand_Format(HAND_PRIMAL_SOLO, HAND_KICKER_NONE,
                                         HAND_CHAINLESS));

        if ((node != NULL) &&
          (CARD_RANK(node->cards.cards[0]) < CARD_RANK_2))
          kicker = HAND_KICKER_SOLO;
      }

      if (node != NULL) {
        CardArray_Concat(&hand->cards, &node->cards);
        Hand_SetKicker(hand->type, kicker);
        HandList_Remove(player->handlist, node);
        break;
      }

      /* no solo nor pair, return with trio */
      break;
    }

    /* pair */
    node = HandList_Find(player->handlist,
                         Hand_Format(HAND_PRIMAL_PAIR, HAND_KICKER_NONE,
                                     HAND_CHAINLESS));

    if ((node != NULL) &&
      (CARD_RANK(node->cards.cards[0]) != CARD_RANK_2)) {
      Hand_Copy(hand, node);
      HandList_Remove(player->handlist, node);
      break;
    }

    /* just play */
    node = player->handlist->first->payload;
    Hand_Copy(hand, node);
    HandList_Remove(player->handlist, node);
  } while (0);

  CardArray_Subtract(&player->cards, &hand->cards);

  return 0;
}

int StandardAI_Beat(void *p, void *g) {
  /*
   * HandList_SearchBeats can search for beat in loop mode
   * but we just simply find a beat here
   */
  int canbeat = 0;
  int i = 0;
  hand_t *tobeat;
  hand_t beat;
  player_t *player = (player_t *) p;
  player_t *prevplayer = NULL;
  player_t *teammate = NULL;
  player_t *landlord = NULL;
  game_t *game = (game_t *) g;

  Hand_Clear(&beat);

  tobeat = &((game_t *) game)->lastHand;

  canbeat = HandList_BestBeat(&player->cards, tobeat, &beat, NULL);

  /*
     canbeat = HandList_SearchBeat(&player->cards, tobeat, &beat);
   */

  /* peasant cooperation */
  prevplayer = &game->players[game->lastplay];

  if (canbeat &&
    (player->identity == PlayerIdentity_Peasant) &&
    (prevplayer->identity == PlayerIdentity_Peasant)) {
    /* find teammate and landlord */
    for (i = 0; i < GAME_PLAYERS; i++) {
      if (game->players[i].identity == PlayerIdentity_Landlord)
        landlord = &game->players[i];

      if ((game->players[i].identity == PlayerIdentity_Peasant) &&
        (game->players[i].seatId !=
          player->seatId))
        teammate = &game->players[i];
    }

    /* don't bomb/nuke teammate */
    if (canbeat &&
      (((beat.type ==
        Hand_Format(HAND_PRIMAL_BOMB, HAND_KICKER_NONE, HAND_CHAINLESS)) ||
        (beat.type ==
          Hand_Format(HAND_PRIMAL_NUKE, HAND_KICKER_NONE,
                      HAND_CHAINLESS)))))
      canbeat = 0;

    if (canbeat && (teammate->cards.length < player->cards.length)) canbeat = 0;
  }

  if (canbeat) {
    CardArray_Subtract(&player->cards, &beat.cards);
    rk_list_clear_destroy(player->handlist);
    player->handlist = HandList_StandardAnalyze(&player->cards);
    Hand_Copy(tobeat, &beat);
  }

  return canbeat;
}
