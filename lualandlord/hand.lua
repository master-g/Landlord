-----------------------------------------------------------------------------
-- hand.lua
-- Landlord
--
-- Create by Master.G on 14-7-23.
-- Copyright (c) 2014 MED. All rights reserved.
-----------------------------------------------------------------------------

require "lualandlord/llinternal";
require "lualandlord/card";

-----------------------------------------------------------------------------
-- hand definition and operations
-----------------------------------------------------------------------------

-- primal hands
ll.HAND_PRIMAL_NONE = 0x00;
ll.HAND_PRIMAL_NUKE = 0x06;
ll.HAND_PRIMAL_BOMB = 0x05;
ll.HAND_PRIMAL_FOUR = 0x04;
ll.HAND_PRIMAL_TRIO = 0x03;
ll.HAND_PRIMAL_PAIR = 0x02;
ll.HAND_PRIMAL_SOLO = 0x01;

-- kicker hands
ll.HAND_KICKER_NONE			= 0x00;
ll.HAND_KICKER_SOLO 		= 0x10;
ll.HAND_KICKER_PAIR 		= 0x20;
ll.HAND_KICKER_DUAL_SOLO 	= 0x30;
ll.HAND_KICKER_DUAL_PAIR	= 0x40;

-- chain
ll.HAND_CHAIN_NONE 			= 0x00;
ll.HAND_CHAIN 				= 0x80;

ll.HAND_NONE 				= 0x00;
ll.HAND_SEARCHER_MASK 		= 0xFF;

-- compare
ll.HAND_CMP_ILLEGAL = -3;
ll.HAND_CMP_LESS 	= -1;
ll.HAND_CMP_EQUAL	= 0;
ll.HAND_CMP_GREATER = 1;

-- basic operations
function ll.Hand_GetPrimal(hand)
	return ll.BIT.band(hand, 0x0F);
end

function ll.Hand_GetKicker(hand)
	return ll.BIT.band(hand, 0x70);
end

function ll.Hand_GetChain(hand)
	return ll.BIT.band(hand, 0x80);
end

function ll.Hand_SetPrimal(hand, primal)
	return ll.BIT.bor(hand, primal);
end

function ll.Hand_SetKicker(hand, kicker)
	return ll.BIT.bor(hand, kicker);
end

function ll.Hand_setChain(hand, kicker)
	return ll.BIT.bor(hand, kicker);
end

function ll.Hand_Format(primal, kicker, chained)
	local hand = ll.BIT.bor(primal, kicker);
	hand = ll.BIT.bor(hand, chained);
	return hand;
end

function ll.Hand_ClearPrimal(hand)
	return ll.BIT.band(hand, 0xF0);
end

function ll.Hand_ClearKicker(hand)
	return ll.BIT.band(hand, 0x8F);
end

function ll.Hand_ClearChain(hand)
	return ll.BIT.band(hand, 0x7F);
end

-----------------------------------------------------------------------------
-- hand is a valid card set
-- cards' format must be like 12345/112233/111222/1112223344/11122234 etc
-- 
-- hand 
-- {
-- 		type   : number
-- 		cards  : CardArray
-- }
-----------------------------------------------------------------------------

ll.HAND_MIN_LENGTH 				= 1;
ll.HAND_MAX_LENGTH 				= 56;
ll.HAND_SOLO_CHAIN_MIN_LENGTH 	= 5;
ll.HAND_PAIR_CHAIN_MIN_LENGTH	= 6;
ll.HAND_TRIO_CHAIN_MIN_LENGTH	= 6;
ll.HAND_FOUR_CHAIN_MIN_LENGTH	= 8;

ll.HANDS_PATTERN_NONE 	= 0   -- place holder
ll.HANDS_PATTERN_1    	= 1   -- 1, solo
ll.HANDS_PATTERN_2_1  	= 2   -- 2, pair
ll.HANDS_PATTERN_2_2  	= 3   -- 2, nuke
ll.HANDS_PATTERN_3    	= 4   -- 3, trio
ll.HANDS_PATTERN_4_1  	= 5   -- bomb
ll.HANDS_PATTERN_4_2  	= 6   -- trio solo
ll.HANDS_PATTERN_5_1  	= 7   -- solo chain
ll.HANDS_PATTERN_5_2  	= 8   -- trio pair
ll.HANDS_PATTERN_6_1  	= 9   -- solo chain
ll.HANDS_PATTERN_6_2  	= 10  -- pair chain
ll.HANDS_PATTERN_6_3  	= 11  -- trio chain
ll.HANDS_PATTERN_6_4  	= 12  -- four dual solo
ll.HANDS_PATTERN_8_1  	= 13  -- solo chain
ll.HANDS_PATTERN_8_2  	= 14  -- pair chain
ll.HANDS_PATTERN_8_3  	= 15  -- trio solo chain
ll.HANDS_PATTERN_8_4  	= 16  -- four dual pair
ll.HANDS_PATTERN_8_5  	= 17  -- four chain
ll.HANDS_PATTERN_9_1  	= 18  -- solo chain
ll.HANDS_PATTERN_9_2  	= 19  -- trio chain
ll.HANDS_PATTERN_10_1 	= 20  -- solo chain
ll.HANDS_PATTERN_10_2 	= 21  -- pair chain
ll.HANDS_PATTERN_10_3 	= 22  -- trio pair chain
ll.HANDS_PATTERN_12_1 	= 23  -- solo chain
ll.HANDS_PATTERN_12_2 	= 24  -- pair chain
ll.HANDS_PATTERN_12_3 	= 25  -- trio chain
ll.HANDS_PATTERN_12_4 	= 26  -- trio solo chain
ll.HANDS_PATTERN_12_5 	= 27  -- four chain
ll.HANDS_PATTERN_12_6 	= 28  -- four dual solo chain
ll.HANDS_PATTERN_16_1 	= 29  -- pair chain
ll.HANDS_PATTERN_16_2 	= 30  -- trio solo chain
ll.HANDS_PATTERN_16_3 	= 31  -- four chain
ll.HANDS_PATTERN_16_4 	= 32  -- four dual pair chain
ll.HANDS_PATTERN_18_1 	= 33  -- pair chain
ll.HANDS_PATTERN_18_2 	= 34  -- trio chain
ll.HANDS_PATTERN_18_3 	= 35  -- four dual solo chain
ll.HANDS_PATTERN_20_1 	= 36  -- pair chain
ll.HANDS_PATTERN_20_2 	= 37  -- trio solo chain
ll.HANDS_PATTERN_20_3 	= 38  -- four chain
ll.HANDS_PATTERN_END 	=  ll.HANDS_PATTERN_20_3

ll._hand_pattern =
{
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     -- 1, solo
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     -- 2, pair
    { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     -- 2, nuke
    { 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     -- 3, trio
    { 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     -- 4, bomb
    { 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     -- 4, trio solo
    { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },     -- 5, solo chain
    { 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     -- 5, trio pair
    { 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },     -- 6, solo chain
    { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     -- 6, pair chain
    { 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     -- 6, trio chain
    { 4, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     -- 6, four dual solo
    { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },     -- 8, solo chain
    { 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0 },     -- 8, pair chain
    { 3, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },     -- 8, trio solo chain
    { 4, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     -- 8, four dual pair
    { 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     -- 8, four chain
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 },     -- 9, solo chain
    { 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     -- 9, trio chain
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },     -- 10, solo chain
    { 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0 },     -- 10, pair chain
    { 3, 3, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0 },     -- 10, trio pair chain
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },     -- 12, solo chain
    { 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0 },     -- 12, pair chain
    { 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0 },     -- 12, trio chain
    { 3, 3, 3, 1, 1, 1, 0, 0, 0, 0, 0, 0 },     -- 12, trio solo chain
    { 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     -- 12, four chain
    { 4, 4, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },     -- 12, four dual solo chain
    { 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0 },     -- 16, pair chain
    { 3, 3, 3, 3, 1, 1, 1, 1, 0, 0, 0, 0 },     -- 16, trio solo chain
    { 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0 },     -- 16, four chain
    { 4, 4, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0 },     -- 16, four dual pair chain
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0 },     -- 18, pair chain
    { 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0 },     -- 18, trio chain
    { 4, 4, 4, 1, 1, 1, 1, 1, 1, 0, 0, 0 },     -- 18, four dual solo chain
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0 },     -- 20, pair chain
    { 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 0, 0 }      -- 20, trio solo chain
};

-- create a empty hand
function ll.Hand_Create()
	local hand = {};
	hand.type = 0;
	hand.cards = ll.CardArray_Create();

	return hand;
end

-- clear a hand
function ll.Hand_Clear(hand)
	hand.type = 0;
	ll.CardArray_Clear(hand.cards);
end

-- copy a hand
function ll.Hand_Copy(hand)
	local clone = {};
	clone.type = hand.type;
	clone.cards = ll.CardArray_Copy(hand.cards);

	return clone;
end

-----------------------------------------------------------------------------
-- parser
-----------------------------------------------------------------------------

-- sort function for rank count array
function ll._Hand_PatternSort(a, b)
	return a > b;
end

-- sort count array by counts
function ll._Hand_SortCount(array)
	table.sort(array, ll._Hand_PatternSort);
end

-- count ranks in a card array
-- return count[rank] = num
function ll.Hand_CountRank(array, sort)
	local count = {};
	local sorted = {};
	for k, v in pairs(array.cards) do
		local rank = ll.Card_Rank(v);
		if count[rank] == nil then
			count[rank] = 1;
		else
			count[rank] = count[rank] + 1;
		end
	end

	if sort ~= nil then
		for k, v in pairs(count) do
			table.insert(sorted, v);
		end

		ll._Hand_SortCount(sorted);
	end

	return count, sorted;
end

-- check if a sorted count array matches a specific pattern
function ll._Hand_PatternMatch(count, pattern)
	for i = 1, #pattern do
		if count[i] ~= pattern[i] then
			return false;
		end
	end

	return true;
end

-- check pattern as 334455 666777 etc
-- | 666 | 777 | 888 | 999 |
-- |  3  |						duplicate : 3
-- |  1  |  2  |  3  |  4  |	expectLength : 4
function ll._Hand_CheckChain(count, duplicate, expectLength)
	local marker = 0;
	local length = 0;

	-- joker and 2 can't chain up
	for i = ll.CARD_RANK_3, ll.CARD_RANK_A do
		if count[i] == duplicate and marker == 0 then
			-- found first match
			marker = i;
		elseif count[i] ~= duplicate and marker ~= 0 then
			length = i - marker;
			break;
		end
	end

	return (length == expectLength);
end


-- distribute cards
-- parames:
-- array : CardArray
-- count : { number }
-- d1    : number
-- d2    : number
-- length : number
-- 
-- for example, _Hand_Distribute(88666644, 422, 4, 2, 8)
-- return 66668844
function ll._Hand_Distribute(array, count, d1, d2, length)
	local num = 0;
	local card = 0;
	local ret = ll.CardArray_Create();
	local temp = ll.CardArray_Create();

	for i = 1, array.length do
		card = array.cards[i];
		num = count[ll.Card_Rank(card)];

		if num == d1 then
			ll.CardArray_PushBack(ret, card);
		elseif num == d2 then
			ll.CardArray_PushBack(temp, card);
		end

		if (ret.length + temp.length) >= length then
			ll.CardArray_Concate(ret, temp);
			break;
		end
	end

	return ret;
end

function ll._Hand_Parse_1(array, count, sorted)
	local hand = ll.Hand_Create();
	-- solo
	hand.cards = ll.CardArray_Copy(array);
	hand.type = ll.Hand_Format(ll.HAND_PRIMAL_SOLO, ll.HAND_KICKER_NONE, ll.HAND_CHAIN_NONE);

	return hand;
end

function ll._Hand_Parse_2(array, count, sorted)
	local hand = ll.Hand_Create();
	-- pair or nuke
	local rank1 = ll.Card_Rank(array.cards[1]);
	local rank2 = ll.Card_Rank(array.cards[2]);

	if rank1 == rank2 then
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_PAIR, ll.HAND_KICKER_NONE, ll.HAND_CHAIN_NONE);
	elseif rank1 == ll.CARD_RANK_R and rank2 == ll.CARD_RANK_r then
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_NUKE, ll.HAND_KICKER_NONE, ll.HAND_CHAIN_NONE);
	end

	hand.cards = ll.CardArray_Copy(array);

	return hand;
end

function ll._Hand_Parse_3(array, count, sorted)
	local hand = ll.Hand_Create();
	-- trio
	local rank1 = ll.Card_Rank(array.cards[1]);
	local rank2 = ll.Card_Rank(array.cards[2]);
	local rank3 = ll.Card_Rank(array.cards[3]);

	if rank1 == rank2 and rank1 == rank3 then
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_TRIO, ll.HAND_KICKER_NONE, ll.HAND_CHAIN_NONE);
		hand.cards = ll.CardArray_Copy(array);
	end

	return hand;
end

function ll._Hand_Parse_4(array, count, sorted)
	local hand = ll.Hand_Create();
	if ll._Hand_PatternMatch(sorted, ll.HANDS_PATTERN_4_1) then
		-- bomb
		hand.cards = ll.CardArray_Copy(array);
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_BOMB, ll.HAND_KICKER_NONE, ll.HAND_CHAIN_NONE);
	elseif ll._Hand_PatternMatch(sorted, ll.HANDS_PATTERN_4_2) then
		-- trio solo
		hand.cards = ll._Hand_Distribute(array, count, 3, 1, 4);
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_TRIO, ll.HAND_KICKER_SOLO, ll.HAND_CHAIN_NONE);
	end

	return hand;
end

function ll._Hand_Parse_5(array, count, sorted)
	local hand = ll.Hand_Create();
	if ll._Hand_PatternMatch(sorted, ll.HANDS_PATTERN_5_2) then
		-- trio pair
		hand.cards = ll._Hand_Distribute(array, count, 3, 2, 5);
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_TRIO, ll.HAND_KICKER_PAIR, ll.HAND_CHAIN_NONE);
	end

	return hand;
end

function ll._Hand_Parse_6(array, count, sorted)
	local hand = ll.Hand_Create();
	if ll._Hand_PatternMatch(sorted, ll.HANDS_PATTERN_6_4) then
		-- four dual solo
		hand.cards = ll._Hand_Distribute(array, count, 4, 1, 6);
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_FOUR, ll.HAND_KICKER_DUAL_SOLO, ll.HAND_CHAIN_NONE);
	end

	return hand;
end

-- length 7 only got solo chain avaliable, skip

function ll._Hand_Parse_8(array, count, sorted)
	local hand = ll.Hand_Create();
	if ll._Hand_PatternMatch(sorted, ll.HANDS_PATTERN_8_3) then
		-- trio solo chain
		hand.cards = ll._Hand_Distribute(array, count, 3, 1, 8);
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_TRIO, ll.HAND_KICKER_SOLO, ll.HAND_CHAIN);
	elseif ll._Hand_PatternMatch(sorted, ll.HANDS_PATTERN_8_4) then
		-- four dual pair
		hand.cards = ll._Hand_Distribute(array, count, 4, 2, 8);
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_FOUR, ll.HAND_KICKER_DUAL_PAIR, ll.HAND_CHAIN_NONE);
	end

	return hand;
end

-- length 9 only got solo chain and trio chain avaliable, skip

function ll._Hand_Parse_10(array, count, sorted)
	local hand = ll.Hand_Create();
	if ll._Hand_PatternMatch(sorted, ll.HANDS_PATTERN_10_3) and ll._Hand_CheckChain(count, 3, 2) then
		-- trio pair chain
		hand.cards = ll._Hand_Distribute(array, count, 3, 2, 10);
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_TRIO, ll.HAND_KICKER_PAIR, ll.HAND_CHAIN);
	end

	return hand;
end

-- length 11 only got solo chain avaliable, skip

function ll._Hand_Parse_12(array, count, sorted)
	local hand = ll.Hand_Create();
	if ll._Hand_PatternMatch(sorted, ll.HANDS_PATTERN_12_4) and ll._Hand_CheckChain(count, 3, 3) then
		-- trio solo chain
		hand.cards = ll._Hand_Distribute(array, count, 3, 1, 12);
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_TRIO, ll.HAND_KICKER_SOLO, ll.HAND_CHAIN);
	elseif ll._Hand_PatternMatch(sorted, ll.HANDS_PATTERN_12_6) and ll._Hand_CheckChain(count, 4, 2) then
		-- four dual solo chain
		hand.cards = ll._Hand_Distribute(array, count, 4, 1, 12);
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_FOUR, ll.HAND_KICKER_DUAL_SOLO, ll.HAND_CHAIN);
	end

	return hand;
end

-- length 13, 15 skipped, length 14 only go pair chain

function ll._Hand_Parse_16(array, count, sorted)
	local hand = ll.Hand_Create();
	if ll._Hand_PatternMatch(sorted, ll.HANDS_PATTERN_16_2) and ll._Hand_CheckChain(count, 3, 4) then
		-- trio solo chain
		hand.cards = ll._Hand_Distribute(array, count, 3, 1, 16);
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_TRIO, ll.HAND_KICKER_SOLO, ll.HAND_CHAIN);
	elseif ll._Hand_PatternMatch(sorted, ll.HANDS_PATTERN_16_4) and ll._Hand_CheckChain(count, 4, 2) then
		-- four dual pair chain
		hand.cards = ll._Hand_Distribute(array, count, 4, 2, 16);
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_FOUR, ll.HAND_KICKER_DUAL_PAIR, ll.HAND_CHAIN);
	end

	return hand;
end

-- length 17 skipped

function ll._Hand_Parse_18(array, count, sorted)
	local hand = ll.Hand_Create();
	if ll._Hand_PatternMatch(sorted, ll.HANDS_PATTERN_18_3) and ll._Hand_CheckChain(count, 4, 3) then
		-- four dual solo chain
		hand.cards = ll._Hand_Distribute(array, count, 4, 1, 18);
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_FOUR, HAND_KICKER_DUAL_SOLO, HAND_CHAIN);
	end

	return hand;
end

-- length 19 skipped

function ll._Hand_Parse_20(array, count, sorted)
	local hand = ll.Hand_Create();
	if ll._Hand_PatternMatch(sorted, ll.HANDS_PATTERN_20_2) and ll._Hand_CheckChain(count, 3, 5) then
		-- trio solo chain
		hand.cards = ll._Hand_Distribute(array, count, 3, 1, 20);
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_TRIO, ll.HAND_KICKER_SOLO, ll.HAND_CHAIN);
	end

	return hand;
end

function ll._Hand_Parse_Default(array, count, sorted)
	return ll.Hand_Create();
end

ll._HandParser = {};
ll._HandParser[ll.HAND_MIN_LENGTH + 0]     = ll._Hand_Parse_1;
ll._HandParser[ll.HAND_MIN_LENGTH + 1]     = ll._Hand_Parse_2;
ll._HandParser[ll.HAND_MIN_LENGTH + 2]     = ll._Hand_Parse_3;
ll._HandParser[ll.HAND_MIN_LENGTH + 3]     = ll._Hand_Parse_4;
ll._HandParser[ll.HAND_MIN_LENGTH + 4]     = ll._Hand_Parse_5;
ll._HandParser[ll.HAND_MIN_LENGTH + 5]     = ll._Hand_Parse_6;
ll._HandParser[ll.HAND_MIN_LENGTH + 6]     = ll._Hand_Parse_Default;
ll._HandParser[ll.HAND_MIN_LENGTH + 7]     = ll._Hand_Parse_8;
ll._HandParser[ll.HAND_MIN_LENGTH + 8]     = ll._Hand_Parse_Default;
ll._HandParser[ll.HAND_MIN_LENGTH + 9]     = ll._Hand_Parse_10;
ll._HandParser[ll.HAND_MIN_LENGTH + 10]    = ll._Hand_Parse_Default;
ll._HandParser[ll.HAND_MIN_LENGTH + 11]    = ll._Hand_Parse_12;
ll._HandParser[ll.HAND_MIN_LENGTH + 12]    = ll._Hand_Parse_Default;
ll._HandParser[ll.HAND_MIN_LENGTH + 13]    = ll._Hand_Parse_Default;
ll._HandParser[ll.HAND_MIN_LENGTH + 14]    = ll._Hand_Parse_Default;
ll._HandParser[ll.HAND_MIN_LENGTH + 15]    = ll._Hand_Parse_16;
ll._HandParser[ll.HAND_MIN_LENGTH + 16]    = ll._Hand_Parse_Default;
ll._HandParser[ll.HAND_MIN_LENGTH + 17]    = ll._Hand_Parse_18;
ll._HandParser[ll.HAND_MIN_LENGTH + 18]    = ll._Hand_Parse_Default;
ll._HandParser[ll.HAND_MIN_LENGTH + 19]    = ll._Hand_Parse_20;

function ll.Hand_Parse(array)
	local count, sorted;
	local hand = ll.Hand_Create();

	-- sort cards
	ll.CardArray_Sort(array);

	-- count ranks
	count, sorted = ll.Hand_CountRank(true);

	-- validate length
	if array.length < ll.HAND_MIN_LENGTH or array.length > ll.HAND_MAX_LENGTH then
		return hand;
	elseif 	array.length >= ll.HAND_SOLO_CHAIN_MIN_LENGTH and
			ll._Hand_CheckChain(count, 1, array.length) then
		-- solo chain
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_SOLO, ll.HAND_KICKER_NONE, ll.HAND_CHAIN);
		hand.cards = ll.CardArray_Copy(array);
	elseif	array.length >= HAND_PAIR_CHAIN_MIN_LENGTH and
			array.length % 2 == 0 and
			
end
