/*
 *  standard_ai.h
 *  Landlord
 *
 *  Created by Master.G on 14-2-28.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#ifndef LANDLORD_STANDARD_AI_H_
#define LANDLORD_STANDARD_AI_H_

#ifdef __cplusplus
extern 'C' {
#endif

/*
 * http://scim.brad.ac.uk/staff/pdf/picowlin/AISB2011.pdf
 * http://en.wikipedia.org/wiki/Monte-Carlo_tree_search
 * http://mcts.ai/about/index.html
 */

/*
 * sort cards, analyze hands etc.
 */
int StandardAI_GetReady(void *player, void *game);

/*
 * decide whether to bid for landlord
 */
int StandardAI_Bid(void *player, void *game);

/*
 * the game will start
 */
int StandardAI_Start(void *player, void *game);

/*
 * free play, result will return by hand_t *
 */
int StandardAI_Play(void *player, void *game);

/*
 * player must play a hand that can beat last player
 * if there is no hand can beat last player, tobeat->type will be 0
 */
int StandardAI_Beat(void *player, void *game);

#ifdef __cplusplus
}
#endif

#endif /* LANDLORD_STANDARD_AI_H_ */
