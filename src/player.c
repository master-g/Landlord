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

#include "player.h"
#include "game.h"
#include "standard_ai.h"
#include "advanced_ai.h"

void Player_SetupStandardAI(player_t *player) {
  player->eventHandlers[Player_Event_GetReady] = StandardAI_GetReady;
  player->eventHandlers[Player_Event_Bid] = StandardAI_Bid;
  player->eventHandlers[Player_Event_Play] = StandardAI_Play;
  player->eventHandlers[Player_Event_Beat] = StandardAI_Beat;
}

void Player_SetupAdvancedAI(player_t *player) {
  player->eventHandlers[Player_Event_GetReady] = AdvancedAI_GetReady;
  player->eventHandlers[Player_Event_Bid] = StandardAI_Bid;
  player->eventHandlers[Player_Event_Play] = StandardAI_Play;
  player->eventHandlers[Player_Event_Beat] = AdvancedAI_Beat;
}

void Player_Destroy(player_t *player) {
  HandList_Destroy(&player->handlist);
  free(player);
}

void Player_Clear(player_t *player) {
  HandList_Destroy(&player->handlist);
  player->handlist = NULL;
  player->identity = PlayerIdentity_Peasant;
  CardArray_Clear(&player->cards);
  CardArray_Clear(&player->record);
}

int Player_HandleEvent(void *p, int event, void *game) {
  player_t *player = (player_t *) p;

  return player->eventHandlers[event](p, game);
}
