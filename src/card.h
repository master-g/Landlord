/*
 *  Header.h
 *  Landlord
 *
 *  Created by Master.G on 14-1-17.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#ifndef LANDLORD_CARD_H_
#define LANDLORD_CARD_H_

#define CARD_RANK_2     0x02
#define CARD_RANK_3     0x03
#define CARD_RANK_4     0x04
#define CARD_RANK_5     0x05
#define CARD_RANK_6     0x06
#define CARD_RANK_7     0x07
#define CARD_RANK_8     0x08
#define CARD_RANK_9     0x09
#define CARD_RANK_T     0x0A
#define CARD_RANK_J     0x0B
#define CARD_RANK_Q     0x0C
#define CARD_RANK_K     0x0D
#define CARD_RANK_A     0x0E
#define CARD_RANK_R     0x0F

#define CARD_SUIT_CLUB      0x10
#define CARD_SUIT_DIAMOND   0x20
#define CARD_SUIT_HEART     0x30
#define CARD_SUIT_SPADE     0x40

#define	CARD_RANK(x)		((x) & 0x0F)
#define CARD_SUIT(x)        ((x) & 0xF0)

#define CARD_RED(x)         (CARD_SUIT(x) == CARD_SUIT_CLUB || CARD_SUIT(x) == CARD_SUIT_HEART)

#define Card_Make(suit, rank) ((suit)|(rank))

int Card_ToString(int card, char *buf, int len);

#endif /* LANDLORD_CARD_H_ */
