require "lua/llinternal";

ll.CARD_RANK_3 = 0x01;
ll.CARD_RANK_4 = 0x02;
ll.CARD_RANK_5 = 0x03;
ll.CARD_RANK_6 = 0x04;
ll.CARD_RANK_7 = 0x05;
ll.CARD_RANK_8 = 0x06;
ll.CARD_RANK_9 = 0x07;
ll.CARD_RANK_T = 0x08;
ll.CARD_RANK_J = 0x09;
ll.CARD_RANK_Q = 0x0A;
ll.CARD_RANK_K = 0x0B;
ll.CARD_RANK_A = 0x0C;
ll.CARD_RANK_2 = 0x0D;
ll.CARD_RANK_r = 0x0E;
ll.CARD_RANK_R = 0x0F;

ll.CARD_RANK_BEG = 0x01;
ll.CARD_RANK_END = 0x0F;

ll.CARD_SUIT_CLUB 		= 0x10;
ll.CARD_SUIT_DIAMOND 	= 0x20;
ll.CARD_SUIT_HEART		= 0x30;
ll.CARD_SUIT_SPADE		= 0x40;

ll.CARD_SET_LENGTH		= 54;

function ll.Card_Rank(card)
	return ll.BIT.band(card, 0x0F);
end
