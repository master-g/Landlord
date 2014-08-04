/*
 *  advanced_ai.h
 *  Landlord
 *
 *  Created by Master.G on 14-3-24.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#ifndef LANDLORD_ADVANCED_AI_H_
#define LANDLORD_ADVANCED_AI_H_

#ifdef __cplusplus
extern 'C' {
#endif

/*
 * sort cards, analyze hands etc.
 */
int AdvancedAI_GetReady(void *player, void *game);

/*
 * decide whether to bid for landlord
 */
int AdvancedAI_Bid(void *player, void *game);

/*
 * free play, result will return by hand_t *
 */
int AdvancedAI_Play(void *player, void *game);

/*
 * player must play a hand that can beat last player
 * if there is no hand can beat last player, tobeat->type will be 0
 */
int AdvancedAI_Beat(void *player, void *game);

#ifdef __cplusplus
}
#endif

#endif /* LANDLORD_ADVANCED_AI_H_ */
