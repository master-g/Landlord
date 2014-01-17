/*
 *  card.c
 *  Landlord
 *
 *  Created by Master.G on 14-1-17.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include <stdio.h>
#include <string.h>

#include "card.h"

#define CARD_STRING_LENGTH 4

char szDIAMOND[] = { 0xE2, 0x99, 0xA6, 0};
char szCLUB[]    = { 0xE2, 0x99, 0xA3, 0};
char szHEART[]   = { 0xE2, 0x99, 0xA5, 0};
char szSPADE[]   = { 0xE2, 0x99, 0xA0, 0};
char szRank[]    = { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'R' };

int Card_ToString(int card, char *buf, int len)
{
    if (buf != NULL && len >= CARD_STRING_LENGTH)
    {
        char *szSuit = NULL;
        int rank = CARD_RANK(card);
        int suit = CARD_SUIT(card);
        
        switch (suit)
        {
            case CARD_SUIT_DIAMOND:
                szSuit = szDIAMOND;
                break;
            case CARD_SUIT_CLUB:
                szSuit = szCLUB;
                break;
            case CARD_SUIT_HEART:
                szSuit = szHEART;
                break;
            case CARD_SUIT_SPADE:
                szSuit = szSPADE;
                break;
                
            default:
                szSuit = "ERR";
                break;
        }
        memcpy(buf, szSuit, CARD_STRING_LENGTH - 1);
        buf[CARD_STRING_LENGTH - 1] = szRank[rank - CARD_RANK_2];
    }
    
    return CARD_STRING_LENGTH;
}

