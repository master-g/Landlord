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

#include "landlord.h"

char szr[] = {'3', '4', '5', '6', '7', '8', '9',
              'T', 'J', 'Q', 'K', 'A', '2', 'R'};

void Count_Print(int* count) {
  int i = 0;

  for (i = CARD_RANK_BEG; i < CARD_RANK_END; i++) {
    printf("%c ", szr[i - 1]);
  }
  printf("\n");

  for (i = CARD_RANK_BEG; i < CARD_RANK_END; i++) {
    printf("%d ", count[i]);
  }
  printf("\n");
}

void test_advanced_hand_analyzer() {
  /*
     const char* str = "♣3 ♣4 ♠5 ♦6 ♠6 ♥7 ♠7 ♦7 ♦8 ♣8 ♣9 ♦9 ♦T";
     const char* str = "♣r ♣2 ♠K ♦K ♠Q ♥J ♠T ♥T ♦T ♣T ♠9 ♥9 ♦8 ♠7 ♥6 ♣6 ♠5 ♦5 ♠4
     ♣4"; const char* str = "♠K ♠Q ♥J ♠9 ♥9 ♦8 ♠7 ♥6 ♣6 ♠5 ♦5"; const char* str
     = "s9 d8 s7 h6 s5 c4";
   */
  const char* str = "♣T ♦9 ♠8 ♥8 ♠7 ♣7 ♦6 ♣6 ♠5 ♣5 ♣4";
  card_array_t cards;
  rk_list_t* hl;

  CardArray_InitFromString(&cards, str);

  CardArray_Print(&cards);

  hl = HandList_AdvancedAnalyze(&cards);

  HandList_Print(hl);

  rk_list_clear_destroy(hl);

  printf("------\n");

  hl = NULL;
  hl = HandList_StandardAnalyze(&cards);
  HandList_Print(hl);
  rk_list_clear_destroy(hl);
}

#ifdef TRY_MULTITHREAD

#include <pthread.h>

struct work_ctx {
  game_t game;
  int peasantwon;
  int landlordwon;
  int index;
  int status;
  pthread_t pid;
};

static struct work_ctx works[4];

void cleanup(void* arg) {
  int i = 0;
  int pwon, lwon;
  for (i = 0; i < 4; i++) {
    if (works[i].status == 0) {
      return;
    }
  }

  pwon = 0;
  lwon = 0;
  for (i = 0; i < 4; i++) {
    pwon += works[i].peasantwon;
    lwon += works[i].landlordwon;
  }

  printf("peasants : %d\n", pwon);
  printf("landlord : %d\n", lwon);

  printf("ended at %ld\n", time(NULL));
}

void work_func(void* arg) {
  int i = 0;
  struct work_ctx* ctx = (struct work_ctx*)arg;
  game_t* game = &ctx->game;

  printf("%lx starts at %d\n", pthread_self(), time(0));

  pthread_cleanup_push(cleanup, arg);

  Game_Init(game);
  ctx->peasantwon = 0;
  ctx->landlordwon = 0;

  for (i = ctx->index; i < ctx->index + 2500; i++) {
    Game_Play(game, i);

    if (game->winner == game->landlord)
      ctx->landlordwon++;
    else
      ctx->peasantwon++;

    Game_Reset(game);
  }

  ctx->status = 1;

  pthread_exit(arg);
  pthread_cleanup_pop(0);
}

void test_game_mt() {
  int i = 0;

  for (i = 0; i < 4; i++) {
    works[i].status = 0;
    works[i].index = 10000 + i * 2500;
    pthread_create(&works[i].pid, NULL, work_func, &works[i]);
  }

  for (i = 0; i < 4; i++) {
    pthread_join(works[i].pid, NULL);
  }
}

#endif

void test_game() {
  int peasantwon = 0;
  int landlordwon = 0;
  int i = 0;

  game_t game;

  printf("start at %ld\n", time(NULL));

  Game_Init(&game);

  for (i = 10000; i < 20000; i++) {
    Game_Play(&game, i);

    if (game.winner == game.landlord)
      landlordwon++;
    else
      peasantwon++;

    Game_Reset(&game);

    if (i % 100 == 0) {
      printf("%.1f%%\n", ((float)i - 10000.0f) / 10000.0f * 100.0f);
    }
  }

  printf("peasants : %d\n", peasantwon);
  printf("landlord : %d\n", landlordwon);

  Game_Clear(&game);

  printf("ended at %ld\n", time(NULL));

  printf("\n");
}

/* "♣3 ♣4 ♠5 ♠6 ♥7 ♦8" */

const char* hand_strings[] = {
    "♣3",                                           /* solo */
    "♣3 ♠3",                                        /* pair */
    "♠r ♠R",                                        /* nuke */
    "♠6 ♥6 ♦6",                                     /* trio */
    "♣3 ♠3 ♥3 ♥7",                                  /* trio solo */
    "♣7 ♠7 ♥7 ♥7",                                  /* bomb */
    "♣3 ♣4 ♠5 ♠6 ♥7",                               /* solo chain */
    "♣3 ♣5 ♠5 ♠3 ♥5",                               /* trio pair */
    "♣3 ♣4 ♠5 ♠6 ♥7 ♦8",                            /* solo chain */
    "♣4 ♠4 ♠5 ♥6 ♥5 ♦6",                            /* pair chain */
    "♣4 ♠4 ♦4 ♥5 ♠5 ♦5",                            /* trio chain */
    "♣4 ♠4 ♦4 ♥4 ♠5 ♦6",                            /* four dual solo */
    "♣3 ♠4 ♦6 ♥8 ♠7 ♦5 ♦9",                         /* solo chain */
    "♣3 ♠4 ♦6 ♥8 ♠7 ♦5 ♦9 ♦T",                      /* solo chain */
    "♣3 ♠3 ♦4 ♥4 ♠6 ♦6 ♦5 ♦5",                      /* pair chain */
    "♣3 ♠3 ♦3 ♥4 ♠6 ♦6 ♦6 ♦9",                      /* trio solo chain */
    "♣3 ♠3 ♦3 ♥3 ♠6 ♦6 ♦9 ♦9",                      /* trio solo chain */
    "♣3 ♠3 ♦3 ♥3 ♠4 ♦4 ♦4 ♦4",                      /* four chain */
    "♣3 ♠3 ♦3 ♥4 ♠4 ♦4 ♦5 ♠5 ♥5",                   /* trio chain */
    "♣3 ♠4 ♦5 ♥6 ♠7 ♦8 ♦9 ♦T ♦J",                   /* solo chain */
    "♣3 ♠4 ♦5 ♥6 ♠7 ♦8 ♦9 ♦T ♦J ♦Q ♦K ♦A ♦2 ♦r ♦R", /* none */
    NULL};

void test_hands() {
  int i = 0;
  card_array_t cards;
  hand_t hand;
  for (; hand_strings[i] != NULL; i++) {
    CardArray_InitFromString(&cards, hand_strings[i]);
    Hand_Parse(&hand, &cards);
    Hand_Print(&hand);
    printf("\n");
  }
}

#define _POSIX_C_SOURCE 201610L

long get_current_time_with_ns(void) {
  struct timespec spec;

  clock_gettime(CLOCK_REALTIME, &spec);
  return spec.tv_nsec;
}

int test_adv() {
  int diff = 0;
  deck_t deck;
  card_array_t cards;
  rk_list_t* hladv = NULL;
  rk_list_t* hlstd = NULL;
  mt19937_t mt;

  Random_Init(&mt, (uint32_t)get_current_time_with_ns());
  Deck_Reset(&deck);
  Deck_Shuffle(&deck, &mt);
  Deck_Deal(&deck, &cards, 11);

  hladv = HandList_AdvancedAnalyze(&cards);
  hlstd = HandList_StandardAnalyze(&cards);

  diff = hlstd->count - hladv->count;
  if (diff >= 2) {
    CardArray_Sort(&cards, NULL);
    CardArray_Print(&cards);
    printf("***************************\n");
    HandList_Print(hlstd);
    printf("***************************\n");
    HandList_Print(hladv);
  }

  rk_list_clear_destroy(hladv);
  rk_list_clear_destroy(hlstd);

  return diff;
}

void do_the_test() {
  const char* str_card = "♣T ♦9 ♠8 ♥8 ♠7 ♣7 ♦6 ♣6 ♠5 ♣5 ♣4";
  card_array_t cards;
  rk_list_t* hl = NULL;
  CardArray_InitFromString(&cards, str_card);
  hl = HandList_AdvancedAnalyze(&cards);
  rk_list_clear_destroy(hl);
}

int main(int argc, const char* argv[]) {
  /*  test_hands(); */
  char* pool = (char*)malloc(512 * 1024);
  memset(pool, 0, 512 * 1024);
  free(pool);
  test_game();

  history_purge();

  /* do_the_test(); */

  /*  test_advanced_hand_analyzer(); */
  memtrack_list_allocations();
  return 0;
}
