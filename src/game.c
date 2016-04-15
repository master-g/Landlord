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
      all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
 */

#include "game.h"

void Game_Init(game_t *game) {
  int i = 0;

  for (i = 0; i < GAME_PLAYERS; i++) {
    Player_SetupStandardAI(&game->players[i]);
    game->players[i].identity = PlayerIdentity_Peasant;
    game->players[i].seatId = i;
    game->players[i].handlist = NULL;
  }

  game->bid = 0;
  game->playerIndex = 0;
  game->landlord = 0;
  game->lastplay = 0;
  game->winner = 0;
  game->status = 0;
  game->phase = 0;

  Deck_Reset(&game->deck);
  Deck_Shuffle(&game->deck, &game->mt);
  CardArray_Clear(&game->cardRecord);
  CardArray_Clear(&game->kittyCards);
  Random_Init(&game->mt, 0);
}

void Game_Clear(game_t *game) {
  int i = 0;

  for (i = 0; i < GAME_PLAYERS; i++) Player_Clear(&game->players[i]);
}

void Game_Destroy(game_t *game) {
  int i = 0;

  for (i = 0; i < GAME_PLAYERS; i++) Player_Clear(&game->players[i]);

  free(game);
}

void Game_Reset(game_t *game) {
  int i = 0;

  for (i = 0; i < GAME_PLAYERS; i++) {
    Player_SetupAdvancedAI(&game->players[i]);
    Player_Clear(&game->players[i]);
  }

  game->bid = 0;
  game->playerIndex = 0;
  game->landlord = 0;
  game->lastplay = 0;
  game->winner = 0;
  game->status = 0;
  game->phase = 0;

  Hand_Clear(&game->lastHand);
  Deck_Reset(&game->deck);
  Deck_Shuffle(&game->deck, &game->mt);
  CardArray_Clear(&game->kittyCards);
  CardArray_Clear(&game->cardRecord);
}

void Game_Play(game_t *game, uint32_t seed) {
  int i = 0;
  int beat = 0;
  int bid = 0;

  Random_Init(&game->mt, seed);

  /* bid */
  /* TODO log */
  game->status = GameStatus_Bid;
  game->bid = 0;
  game->highestBidder = -1;

  while (game->status == GameStatus_Bid) {
    game->playerIndex = Random_Int32(&game->mt) % GAME_PLAYERS;

    for (i = 0; i < GAME_PLAYERS; i++) {
      Deck_Deal(&game->deck, &Game_GetCurrentPlayer(game)->cards,
                GAME_HAND_CARDS);
      bid =
          Player_HandleEvent(Game_GetCurrentPlayer(game), Player_Event_Bid, game);

      if (bid > game->bid) {
        DBGLog ("\nPlayer ---- %d ---- bid for %d\n", game->playerIndex, bid);
        game->highestBidder = game->playerIndex;
        game->bid = bid;
      }

      Game_IncPlayerIndex(game);
    }

    /* check if bid stage is done */
    if (game->bid == 0) {
      Deck_Reset(&game->deck);
    }
    else {
      /* setup landlord, game start! */
      game->landlord = game->highestBidder;
      game->players[game->landlord].identity = PlayerIdentity_Landlord;
      game->playerIndex = game->landlord;
      game->phase = Phase_Play;
      Deck_Deal(&game->deck, &game->kittyCards, GAME_REST_CARDS);
      CardArray_Concate(&game->players[game->landlord].cards, &game->kittyCards);
      game->status = GameStatus_Ready;

      /* Player_SetupAdvancedAI(&game->players[game->landlord]); */
    }
  }

  /*
     game->landlord = Random_Int32(&game->mt) % 3;
     game->players[game->landlord].identity = PlayerIdentity_Landlord;
     Player_SetupAdvancedAI(&game->players[game->landlord]);

     Deck_Shuffle(&game->deck, &game->mt);



     game->status = GameStatus_Ready;
     game->playerIndex = game->landlord;
     game->phase = Phase_Play;
   */

  for (i = 0; i < GAME_PLAYERS; i++)
    Player_HandleEvent(&game->players[i],
                       Player_Event_GetReady,
                       game);

  /* game play */
  while (game->status != GameStatus_Over) {
    if (game->phase == Phase_Play) {
      Player_HandleEvent(Game_GetCurrentPlayer(game), Player_Event_Play, game);
      game->lastplay = game->playerIndex;
      game->phase = Phase_Query;

      CardArray_Concate(&game->cardRecord, &game->lastHand.cards);

      DBGLog ("\nPlayer ---- %d ---- played\n", game->playerIndex);
      Hand_Print(&game->lastHand);
    }
    else if ((game->phase == Phase_Query) || (game->phase == Phase_Pass)) {
      beat = Player_HandleEvent(Game_GetCurrentPlayer(
                                    game), Player_Event_Beat, game);

      /* has beat in this phase */
      if (beat == 0) {
        /* two player pass */
        if (game->phase == Phase_Pass) game->phase = Phase_Play;
        else game->phase = Phase_Pass;

        DBGLog ("\nPlayer ---- %d ---- passed\n", game->playerIndex);
      }
      else {
        game->lastplay = game->playerIndex;
        game->phase = Phase_Query;
        CardArray_Concate(&game->cardRecord, &game->lastHand.cards);

        DBGLog ("\nPlayer ---- %d ---- beat\n", game->playerIndex);
        Hand_Print(&game->lastHand);
      }
    }

    Game_IncPlayerIndex(game);

    /* check if there is player win */
    for (i = 0; i < GAME_PLAYERS; i++) {
      if (game->players[i].cards.length == 0) {
        game->status = GameStatus_Over;
        game->winner = i;

        DBGLog ("\nPlayer ++++ %d ++++ wins!\n", i);
        break;
      }
    }
  }
}
