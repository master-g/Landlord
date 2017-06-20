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

#ifndef LANDLORD_ADVANCED_AI_H_
#define LANDLORD_ADVANCED_AI_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * sort cards, analyze hands etc.
 */
int AdvancedAI_GetReady(void* player, void* game);

/*
 * decide whether to bid for landlord
 */
int AdvancedAI_Bid(void* player, void* game);

/*
 * free play, result will return by hand_t *
 */
int AdvancedAI_Play(void* player, void* game);

/*
 * player must play a hand that can beat last player
 * if there is no hand can beat last player, tobeat->type will be 0
 */
int AdvancedAI_Beat(void* player, void* game);

#ifdef __cplusplus
}
#endif

#endif /* LANDLORD_ADVANCED_AI_H_ */
