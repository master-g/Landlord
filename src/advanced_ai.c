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

#include "advanced_ai.h"
#include "game.h"

/*
 * sort cards, analyze hands etc.
 */
int AdvancedAI_GetReady(void* p, void* game) {
  player_t* player = (player_t*)p;

  CardArray_Sort(&player->cards, NULL);
  CardArray_Copy(&player->record, &player->cards);
#if (PRINT_GAME_LOG == 1)
  CardArray_Print(&player->record);
#endif /* ifdef PRINT_GAME_LOG */
  player->handlist = HandList_AdvancedAnalyze(&player->cards);

  return 0;
}

/*
 * decide whether to bid for landlord
 */
int AdvancedAI_Bid(void* p, void* g) {
  return 0;
}

/*
 * free play, result will return by hand_t *
 */
int AdvancedAI_Play(void* p, void* game) {
  /* TODO */

  return 0;
}

/*
 * player must play a hand that can beat last player
 * if there is no hand can beat last player, tobeat->type will be 0
 */
int AdvancedAI_Beat(void* p, void* g) {
  /*
   * HandList_SearchBeats can search for beat in loop mode
   * but we just simply find a beat here
   */
  int canbeat = 0;
  int i = 0;
  hand_t* tobeat;
  hand_t beat;
  player_t* player = (player_t*)p;
  player_t* prevplayer = NULL;
  player_t* teammate = NULL;
  player_t* landlord = NULL;
  game_t* game = (game_t*)g;

  Hand_Clear(&beat);

  tobeat = &((game_t*)game)->lastHand;

  canbeat = HandList_BestBeat(&player->cards, tobeat, &beat,
                              HandList_AdvancedEvaluator);

  /*
     canbeat = HandList_SearchBeat(&player->cards, tobeat, &beat);
   */

  /* peasant cooperation */
  prevplayer = &game->players[game->lastplay];

  if (canbeat && (player->identity == PlayerIdentity_Peasant) &&
      (prevplayer->identity == PlayerIdentity_Peasant)) {
    /* find teammate and landlord */
    for (i = 0; i < GAME_PLAYERS; i++) {
      if (game->players[i].identity == PlayerIdentity_Landlord)
        landlord = &game->players[i];

      if ((game->players[i].identity == PlayerIdentity_Peasant) &&
          (game->players[i].seatId != player->seatId))
        teammate = &game->players[i];
    }

    /* don't bomb/nuke teammate */
    if (canbeat &&
        (((beat.type ==
           Hand_Format(HAND_PRIMAL_BOMB, HAND_KICKER_NONE, HAND_CHAINLESS)) ||
          (beat.type ==
           Hand_Format(HAND_PRIMAL_NUKE, HAND_KICKER_NONE, HAND_CHAINLESS)))))
      canbeat = 0;

    if (canbeat && (teammate->cards.length < player->cards.length))
      canbeat = 0;
  }

  if (canbeat) {
    CardArray_Subtract(&player->cards, &beat.cards);
    rk_list_clear_destroy(player->handlist);
    player->handlist = HandList_AdvancedAnalyze(&player->cards);
    Hand_Copy(tobeat, &beat);
  }

  return canbeat;
}
