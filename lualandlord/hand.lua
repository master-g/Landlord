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

	for i = ll.CARD_RANK_3, ll.CARD_RANK_R do
		count[i] = 0;
		sorted[i] = 0;
	end

	for k, v in pairs(array.cards) do
		local rank = ll.Card_Rank(v);
		count[rank] = count[rank] + 1;
	end

	if sort ~= nil then
		for k, v in pairs(count) do
			table.insert(sorted, v);
		end

		ll._Hand_SortCount(sorted);
	end

	return count, sorted;
end

-- create a count array
function ll._Hand_CreateCount()
	local count = {};
	for i = ll.CARD_RANK_3, ll.CARD_RANK_R do
		count[i] = 0;
	end

	return count;
end

function ll._Hand_CopyCount(count)
	local clone = {};
	for k, v in pairs(count) do
		clone[k] = v;
	end

	return clone;
end

-- check if a sorted count array matches a specific pattern
function ll._Hand_PatternMatch(count, pattern)
	for i = 1, #ll._hand_pattern[pattern] do
		if count[i] ~= ll._hand_pattern[pattern][i] then
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
	count, sorted = ll.Hand_CountRank(array, true);

	-- validate length
	if array.length < ll.HAND_MIN_LENGTH or array.length > ll.HAND_MAX_LENGTH then
		return hand;
	elseif 	array.length >= ll.HAND_SOLO_CHAIN_MIN_LENGTH and
			ll._Hand_CheckChain(count, 1, array.length) then
		-- solo chain
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_SOLO, ll.HAND_KICKER_NONE, ll.HAND_CHAIN);
		hand.cards = ll.CardArray_Copy(array);
	elseif	array.length >= ll.HAND_PAIR_CHAIN_MIN_LENGTH and
			array.length % 2 == 0 and
			ll._Hand_CheckChain(count, 2, array.length / 2) then
		-- pair chain
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_PAIR, ll.HAND_KICKER_NONE, ll.HAND_CHAIN);
		hand.cards = ll.CardArray_Copy(array);
	elseif 	array.length >= ll.HAND_TRIO_CHAIN_MIN_LENGTH and
			array.length % 3 == 0 and
			ll._Hand_CheckChain(count, 3, array.length / 3) then
		-- trio chain
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_TRIO, ll.HAND_KICKER_NONE, ll.HAND_CHAIN);
		hand.cards = ll.CardArray_Copy(array);
	elseif 	array.length >= ll.HAND_FOUR_CHAIN_MIN_LENGTH and
			array.length % 4 == 0 and
			ll._Hand_CheckChain(count, 4, array.length / 4) then
		-- four chain
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_FOUR, ll.HAND_KICKER_NONE, ll.HAND_CHAIN);
		hand.cards = ll.CardArray_Copy(array);
	else
		-- other types
		hand = ll._HandParser[array.length](array, count, sorted);
	end

	return hand;
end

-----------------------------------------------------------------------------
-- comparators
-----------------------------------------------------------------------------

-- one of hand1, hand2 must be bomb or nuke
function ll._Hand_CompareBomb(hand1, hand2)
	local ret = ll.HAND_CMP_ILLEGAL;

	-- same type same cards, equal
	if (hand1.type == hand2.type) and (hand1.cards.cards[1] == hand2.cards.cards[1]) then
		ret = ll.HAND_CMP_EQUAL;
	elseif (hand1.type == ll.HAND_PRIMAL_BOMB) and (hand2.type == ll.HAND_PRIMAL_BOMB) then
		if ll.Card_Rank(hand1.cards.cards[1]) > ll.Card_Rank(hand2.cards.cards[1]) then
			ret = ll.HAND_CMP_GREATER;
		else
			ret = ll.HAND_CMP_LESS;
		end
	else
		if ll.Hand_GetPrimal(hand1.type) > ll.Hand_GetPrimal(hand2.type) then
			ret = ll.HAND_CMP_GREATER;
		else
			ret = ll.HAND_CMP_LESS;
		end
	end

	return ret;
end

-- compare two hands
function ll.Hand_Compare(hand1, hand2)
	local ret = ll.HAND_CMP_ILLEGAL;

	-- different hand type
	-- check for bomb and nuke
	if hand1.type ~= hand2.type then
		if 	hand1.type ~= ll.HAND_PRIMAL_NUKE and
			hand1.type ~= ll.HAND_PRIMAL_BOMB and 
			hand2.type ~= ll.HAND_PRIMAL_NUKE and
			hand2.type ~= ll.HAND_PRIMAL_BOMB then
			ret = ll.HAND_CMP_ILLEGAL;
		else
			ret = ll._Hand_CompareBomb(hand1, hand2);
		end
	else
		-- same hand type and with no bombs
		if hand1.cards.length ~= hand2.cards.length then
			ret = ll.HAND_CMP_ILLEGAL;
		else
			-- same hand type and same length
			if ll.Card_Rank(hand1.cards.cards[1]) == ll.Card_Rank(hand2.cards.cards[1]) then
				ret = ll.HAND_CMP_EQUAL;
			else
				if ll.Card_Rank(hand1.cards.cards[1]) > ll.Card_Rank(hand2.cards.cards[1]) then
					ret = ll.HAND_CMP_GREATER;
				else 
					ret = ll.HAND_CMP_LESS;
				end
			end
		end
	end

	return ret;
end

-- convert hand to string
function ll.Hand_ToString(hand)
	local str = "Hand type: [";

	local primal = ll.Hand_GetPrimal(hand.type);
	local kicker = ll.Hand_GetKicker(hand.type);

	-- primal
	if primal == ll.HAND_PRIMAL_NONE then
		str = str .. "none ";
	elseif primal == ll.HAND_PRIMAL_SOLO then
		str = str .. "solo ";
	elseif primal == ll.HAND_PRIMAL_PAIR then
		str = str .. "pair ";
	elseif primal == ll.HAND_PRIMAL_TRIO then
		str = str .. "trio ";
	elseif primal == ll.HAND_PRIMAL_FOUR then
		str = str .. "four ";
	elseif primal == ll.HAND_PRIMAL_BOMB then
		str = str .. "bomb ";
	elseif primal == ll.HAND_PRIMAL_NUKE then
		str = str .. "nuke ";
	end

	-- kicker
	if kicker == ll.HAND_KICKER_NONE then
		str = str .. "none";
	elseif kicker == ll.HAND_KICKER_SOLO then
		str = str .. "solo";
	elseif kicker == ll.HAND_KICKER_PAIR then
		str = str .. "pair";
	elseif kicker == ll.HAND_KICKER_DUAL_SOLO then
		str = str .. "dual solo";
	elseif kicker == ll.HAND_KICKER_DUAL_PAIR then
		str = str .. "dual pair";
	end

	-- chain
	if ll.Hand_GetChain(hand.type) == ll.HAND_CHAIN then
		str = str .. " chain";
	end

	str = str .. "]\n" .. ll.CardArray_ToString(hand.cards);

	return str;
end

-----------------------------------------------------------------------------
-- hand list
-----------------------------------------------------------------------------

-- push a hand into the front of a handlist, if handlist is nil , new handlist will be created
function ll.HandList_PushFront(list, hand)
	if list == nil then
		list = {};
	end

	table.insert(list, 1, hand);

	return list;
end

-- remove a hand from list
function ll.HandList_Remove(list, hand)
	for i = 1, #list do
		if list[i] == hand then
			table.remove(list, i);
			return;
		end
	end
end

-- concate two handlist
function ll.HandList_Concate(head, tail)
	if head == nil then
		head = {};
	end

	for k, v in pairs(tail) do
		table.insert(head, v);
	end

	return head;
end

-- find function
function ll.HandList_Find(handlist, handtype)
	local hand = nil;
	for k, v in pairs(handlist) do
		if v.type == handtype then
			hand = v;
			break;
		end
	end

	return hand;
end

-----------------------------------------------------------------------------
-- beat search
-- 
-- hand_ctx
-- {
-- 		count 	: { number }
-- 		cards 	: CardArray
--		rcards	: CardArray
-- }
-----------------------------------------------------------------------------

-- create hand context from card array
function ll.HandContext_Create(array)
	local handctx = {};

	handctx.count = ll.Hand_CountRank(array);
	handctx.cards = ll.CardArray_Copy(array);
	ll.CardArray_Sort(handctx.cards);
	handctx.rcards = ll.CardArray_Copy(handctx.cards);
	ll.CardArray_Reverse(handctx.rcards);

	return handctx;
end

-- copy hand context
function ll._HandContext_Copy(ctx)
	local clone = {};
	clone.count = ll._Hand_CopyCount(ctx.count);
	clone.cards = ll.CardArray_Copy(ctx.cards);
	clone.rcards = ll.CardArray_Copy(ctx.rcards);

	return clone;
end

--[[
-- next_comb(int comb[], int k, int n)
-- Generates the next combination of n elements as k after comb
-- 
-- comb => the previous combination ( use (0, 1, 2, ..., k) for first)
-- k => the size of the subsets to generate
-- n => the size of the original set
-- 
-- Returns: 1 if a valid combination was found
-- 0, otherwise
--]]
function ll._next_comb(comb, k, n)
	local i = k;
	comb[i] = comb[i] + 1;

	while i > 1 and (comb[i] >= n - k + 1 + i) do
		i = i - 1;
		comb[i] = comb[i] + 1;
	end

	if comb[1] > n - k + 1 then 	-- Combination (n-k, n-k+1, ..., n) reached
		return false;				-- No more combinations can be generated
	end

	-- comb now looks like (..., x, n, n, n, ..., n).
    -- Turn it into (..., x, x + 1, x + 2, ...)
	for j = i + 1, k do
		comb[j] = comb[j - 1] + 1;
	end

	return true;
end

function ll._HandList_SearchBeat_Primal(handctx, tobeat, beat, primal)
	local i = 0;
	local canbeat = false;
	local count = nil;
	local rank = 0;
	local temp = null;
	local tobeattype = tobeat.type;

	count = handctx.count;
	temp = handctx.rcards;

	rank = ll.Card_Rank(tobeat.cards.cards[1]);

	-- search for primal
	for i = 1, temp.length do
		local searchRank = ll.Card_Rank(temp.cards[i]);
		if searchRank > rank and count[searchRank] >= primal then
			ll.Hand_Clear(beat);
			beat.type = tobeattype;
			ll.CardArray_PushBackCards(beat.cards, temp, i, primal);
			canbeat = true;
			break;
		end
	end

	return canbeat;
end

function ll._HandList_SearchBeat_Bomb(handctx, tobeat, beat)
	local canbeat = false;
	local count = handctx.count;
	local cards = handctx.cards;

	-- can't beat nuke
	if tobeat.type == ll.HAND_PRIMAL_NUKE then
		return false;
	end

	-- search for higher rank bomb
	if tobeat.type == ll.HAND_PRIMAL_BOMB then
		canbeat = ll._HandList_SearchBeat_Primal(handctx, tobeat, beat, 4);
	else
		-- tobeat is not a nuke or bomb, search a bomb to beat it
		for i = 1, cards.length do
			if count[ll.Card_Rank(cards.cards[i])] == 4 then
				canbeat = true;
				ll.Hand_Clear(beat);
				ll.CardArray_CopyRank(beat.cards, cards, ll.Card_Rank(cards.cards[i]));
				break;
			end
		end
	end

	-- search for nuke
	if not canbeat then
		if count[ll.CARD_RANK_r] ~= 0 and count[ll.CARD_RANK_R] ~= 0 then
			canbeat = true;
			ll.Hand_Clear(beat);
			beat.type = ll.HAND_PRIMAL_NUKE;
			ll.CardArray_CopyRank(beat.cards, cards, ll.CARD_RANK_R);
			ll.CardArray_CopyRank(beat.cards, cards, ll.CARD_RANK_r);
		end
	else
		beat.type = ll.HAND_PRIMAL_BOMB;
	end

	return canbeat;
end

--[[
-- for a standard 54 card set, each rank has four cards
-- so it is impossible for two trio of the same rank at the same time
-- 
-- a) player_1 SEARCH_BEAT player_2 : impossible for 333 vs 333
-- 
-- BUT

-- b) player_1 SEARCH_BEAT_LOOP player_1_prev_beat : possible for 333 vs 333
--]]
function ll._HandList_SearchBeat_TrioKicker(handctx, tobeat, beat, kick)
	local canbeat = false;
	local cantriobeat = false;
	local tobeattype = tobeat.type;
	local count = handctx.count;
	local temp = ll.CardArray_Copy(handctx.rcards);

	local htrio = ll.Hand_Create();
	local hkick = ll.Hand_Create();
	local htriobeat = ll.Hand_Create();
	local hkickbeat = ll.Hand_Create();

	-- copy hands
	ll.CardArray_PushBackCards(htrio.cards, tobeat.cards, 1, 3);
	ll.CardArray_PushBackCards(hkick.cards, tobeat.cards, 4, kick);

	-- same rank trio, case b)
	if ll.CardArray_Contains(temp, htrio.cards) then
		-- keep trio beat
		htriobeat.cards = ll.CardArray_Copy(htrio.cards);
		ll.CardArray_RemoveRank(temp, ll.Card_Rank(htriobeat.cards.cards[1]));

		-- search for a higher kicker
		-- round 1 : only search those count[rank] == kick
		for i = 1, temp.length do
			local ranktemp = ll.Card_Rank(temp.cards[i]);
			if count[ranktemp] >= kick and ranktemp > ll.Card_Rank(hkick.cards.cards[1]) then
				ll.CardArray_Clear(hkickbeat.cards);
				ll.CardArray_PushBackCards(hkickbeat.cards, temp, i, kick);
				canbeat = true;
				break;
			end
		end

		-- if kicker can't beat, restore trio
		if not canbeat then
			ll.CardArray_Clear(htriobeat.cards);
			temp = ll.CardArray_Copy(handctx.rcards);
		end
	end

	-- same rank trio not found
	-- OR
	-- same rank trio found, but kicker can't beat
	if not canbeat then
		cantriobeat = ll._HandList_SearchBeat_Primal(handctx, htrio, htriobeat, ll.HAND_PRIMAL_TRIO);
		if cantriobeat then
			-- trio beat found, search for kicker beat

			-- remove trio from temp
			ll.CardArray_RemoveRank(temp, ll.Card_Rank(htriobeat.cards.cards[1]));

			-- search for a kicker
			for i = 1, temp.length do
				if count[ll.Card_Rank(temp.cards[i])] >= kick then
					ll.CardArray_PushBackCards(hkickbeat.cards, temp, i, kick);
					canbeat = true;
					break;
				end
			end
		end
	end

	-- beat
	if canbeat then
		ll.Hand_Clear(beat);
		ll.CardArray_Concate(beat.cards, htriobeat.cards);
		ll.CardArray_Concate(beat.cards, hkickbeat.cards);
		beat.type = tobeattype;
	end

	return canbeat;
end

function ll._HandList_SearchBeat_Chain(handctx, tobeat, beat, duplicate)
	local canbeat = false;
	local found = false;
	local tobeattype = tobeat.type;
	local chainlength = tobeat.cards.length / duplicate;
	local count = handctx.count;
	local cards = handctx.cards;
	local temp = ll.CardArray_Create();
	local footer = ll.Card_Rank(tobeat.cards.cards[tobeat.cards.length]);

	-- search for beat chain in rank counts
	for i = footer, ll.CARD_RANK_2 - chainlength do 
		found = true;

		for j = 1, chainlength do
			-- check if chain breaks
			if count[i + j] < duplicate then
				found = false;
				break;
			end
		end

		local k = 0;
		if found then
			footer = i;			-- beat footer rank
			k = duplicate;		-- how many cards need for each rank

			i = card.length;
			while i >= 0 and chainlength > 0 do
				if ll.Card_Rank(cards.cards[i]) == footer then
					ll.CardArray_PushFront(temp, cards.cards[i]);
					k = k - 1;

					if k == 0 then
						k = duplicate;
						chainlength = chainlength - 1;
						footer = footer + 1;
					end
				end

				i = i - 1;
			end

			break;
		end
	end

	if found then
		beat.type = tobeattype;
		beat.cards = temp;
		canbeat = true;
	end

	return canbeat;
end

function ll._HandList_SearchBeat_TrioKickerChain(handctx, tobeat, beat, kc)
	local canbeat = false;
	local i = 0;
	local j = 0;
	local chainlength = 0;
	local cantriobeat = false;
	local tobeattype = tobeat.type;
	local count = ll._Hand_CopyCount(handctx.count);
	local kickcount = ll._Hand_CreateCount();
	local combrankmap = ll._Hand_CreateCount();
	local rankcombmap = ll._Hand_CreateCount();
	local comb = ll._Hand_CreateCount();
	local temp = ll.CardArray_Copy(handctx.rcards);
	local htrio = ll.Hand_Create();
	local hkick = ll.Hand_Create();
	local htriobeat = ll.Hand_Create();
	local hkickbeat = ll.Hand_Create();

	-- setup variables
	chainlength = tobeattype.cards.length / (ll.HAND_PRIMAL_TRIO + kc);

	-- copy tobeat cards
	ll.CardArray_PushBackCards(htrio.cards, tobeat.cards, 1, 3 * chainlength);
	ll.CardArray_PushBackCards(hkick.cards, tobeat.cards, 3 * chainlength + 1, kc * chainlength);

	htrio.type = ll.Hand_Format(ll.HAND_PRIMAL_TRIO, ll.HAND_KICKER_NONE, ll.HAND_CHAIN);

	-- self beat, see _HandList_SearchBeat_TrioKicker
	if ll.CardArray_Contains(temp, htrio.cards) then
		local n = 0;	-- combination total
		-- remove trio from kickcout
		kickcount = ll._Hand_CopyCount(count);
		for i = 1, htrio.cards.length, 3 do
			kickcount[ll.Card_Rank(htrio.cards.cards[i])] = 0;
		end

		-- remove count < kc and calculate n
		for i = ll.CARD_RANK_3, ll.CARD_RANK_R do
			if kickcount[i] < kc then
				kickcount[i] = 0;
			else
				n = n + 1;
			end
		end

		-- setup comb-rank and rank-comb map
		j = 0;
		for i = 1, hkick.cards.length, kc do
			j = j + 1;
			comb[j] = rankcombmap[ll.Card_Rank(hkick.cards.cards[i])];
		end

		-- find next combination
		if ll._next_comb(comb, chainlength, n) then
			-- next combination found, copy kickers
			for i = 1, chainlength do
				local rank = combrankmap[comb[i]];
				for j = 1, temp.length do
					if ll.Card_Rank(temp.cards[j]) == rank then
						ll.CardArray_PushBackCards(hkickbeat.cards, temp, j, kc);
						break;
					end
				end
			end

			canbeat = true;
			-- copy trio to beat
			ll.CardArray_Concate(htriobeat.cards, htrio.cards);
			CardArray_Sort(hkickbeat.cards);
		end
	end

	-- can't find same rank trio chain, search for higher rank trio
	if not canbeat then
		-- restore rank count
		count = ll._Hand_CopyCount(handctx.count);
		cantriobeat = ll._HandList_SearchBeat_Chain(handctx, htrio, htriobeat, 3);
		if cantriobeat then
			-- higher rank trio chain found, search for kickers

			-- remove trio from temp
			for i = 1, htriobeat.cards.length, 3 do
				ll.CardArray_RemoveRank(temp, ll.Card_Rank(htriobeat.cards.cards[i]));
				count[ll.Card_Rank(htriobeat.cards.cards[1])] = 0;
			end

			for j = 1, chainlength do
				for i = 1, temp.length do
					if count[ll.Card_Rank(temp.cards[i])] >= kc then
						ll.CardArray_PushBackCards(hkickbeat.cards, temp, i, kc);
						ll.CardArray_RemoveRank(temp, ll.Card_Rank(temp.cards[i]));
						break;
					end
				end
			end

			if hkickbeat.cards.length == kc * chainlength then
				canbeat = true;
			end
		end
	end

	-- final
	if canbeat then
		ll.Hand_Clear(beat);
		ll.CardArray_Concate(beat.cards, htriobeat.cards);
		ll.CardArray_Concate(beat.cards, hkickbeat.cards);
		beat.type = tobeattype;
	end

	return canbeat;
end

function ll._HandList_SearchBeat(cards, tobeat, beat)
	local canbeat = false;
	local tobeattype = tobeat.type;
	-- setup search context
	local handctx = ll.HandContext_Create(cards);

	-- start search
	if tobeattype == ll.HAND_PRIMAL_SOLO then
		canbeat = ll._HandList_SearchBeat_Primal(handctx, tobeat, beat, ll.HAND_PRIMAL_SOLO);
	elseif tobeattype == ll.HAND_PRIMAL_PAIR then
		canbeat = ll._HandList_SearchBeat_Primal(handctx, tobeat, beat, ll.HAND_PRIMAL_PAIR);
	elseif tobeattype == ll.HAND_PRIMAL_TRIO then
		canbeat = ll._HandList_SearchBeat_Primal(handctx, tobeat, beat, ll.HAND_PRIMAL_TRIO);
	elseif tobeattype == ll.Hand_Format(ll.HAND_PRIMAL_TRIO, ll.HAND_KICKER_PAIR, ll.HAND_CHAIN_NONE) then
		canbeat = ll._HandList_SearchBeat_TrioKicker(handctx, tobeat, beat, ll.HAND_PRIMAL_PAIR);
	elseif tobeattype == ll.Hand_Format(ll.HAND_PRIMAL_TRIO, ll.HAND_KICKER_SOLO, ll.HAND_CHAIN_NONE) then
		canbeat = ll._HandList_SearchBeat_TrioKicker(handctx, tobeat, beat, ll.HAND_PRIMAL_SOLO);
	elseif tobeattype == ll.Hand_Format(ll.HAND_PRIMAL_SOLO, ll.HAND_KICKER_NONE, ll.HAND_CHAIN) then
		canbeat = ll._HandList_SearchBeat_Chain(handctx, tobeat, beat, ll.HAND_PRIMAL_SOLO);
	elseif tobeattype == ll.Hand_Format(ll.HAND_PRIMAL_PAIR, ll.HAND_KICKER_NONE, ll.HAND_CHAIN) then
		canbeat = ll._HandList_SearchBeat_Chain(handctx, tobeat, beat, ll.HAND_PRIMAL_PAIR);
	elseif tobeattype == ll.Hand_Format(ll.HAND_PRIMAL_TRIO, ll.HAND_KICKER_NONE, ll.HAND_CHAIN) then
		canbeat = ll._HandList_SearchBeat_Chain(handctx, tobeat, beat, ll.HAND_PRIMAL_TRIO);
	elseif tobeattype == ll.Hand_Format(ll.HAND_PRIMAL_FOUR, ll.HAND_KICKER_NONE, ll.HAND_CHAIN) then
		canbeat = ll._HandList_SearchBeat_Chain(handctx, tobeat, beat, ll.HAND_PRIMAL_FOUR);
	elseif tobeattype == ll.Hand_Format(ll.HAND_PRIMAL_TRIO, ll.HAND_KICKER_PAIR, ll.HAND_CHAIN) then
		canbeat = ll._HandList_SearchBeat_TrioKickerChain(handctx, tobeat, beat, ll.HAND_PRIMAL_PAIR);
	elseif tobeattype == ll.Hand_Format(ll.HAND_PRIMAL_TRIO, ll.HAND_KICKER_SOLO, ll.HAND_CHAIN) then
		canbeat = ll._HandList_SearchBeat_TrioKickerChain(handctx, tobeat, beat, ll.HAND_KICKER_SOLO);
	end

	-- search for bomb/nuke
	if not canbeat then
		canbeat = ll._HandList_SearchBeat_Bomb(handctx, tobeat, beat);
	end

	return canbeat;
end

-- search for beat, result will be store in beat
-- 1, if [beat->type] != 0, then search [new beat] > [beat]
-- 2, search [beat] > [tobeat], then store in [beat]
function ll.HandList_SearchBeat(cards, tobeat, beat)
	-- already in search loop, continue
	if beat.type ~= 0 then
		return ll._HandList_SearchBeat(cards, beat, beat);
	else
		return ll._HandList_SearchBeat(cards, tobeat, beat);
	end
end

function ll.HandList_SearchBeatList(cards, tobeat)
	local hl = {};
	local htobeat = ll.Hand_Copy(tobeat);
	local beat = ll.Hand_Create();
	local canbeat = false;

	repeat
		canbeat = ll._HandList_SearchBeat(cards, htobeat, beat);
		if canbeat then
			htobeat = ll.Hand_Copy(beat);
			local temp = ll.Hand_Copy(beat);
			table.insert(hl, 1, temp);
		end
	until not canbeat;

	return hl;
end

-----------------------------------------------------------------------------
-- hand analyzer
-----------------------------------------------------------------------------

-- extract hands like 34567 / 334455 / 333444555 etc
-- array is a processed card array holds count[rank] == duplicate
function ll._HandList_ExtractConsecutive(handlist, array, duplicate)
	if array.length == 0 then
		return;
	end

	local cardnum = array.length / duplicate;
	local lastrank = ll.Card_Rank(array.cards[1]);
	local hand = nil;
	local primal = { ll.HAND_PRIMAL_SOLO, ll.HAND_PRIMAL_PAIR, ll.HAND_PRIMAL_TRIO };
	local chainlen = { ll.HAND_SOLO_CHAIN_MIN_LENGTH, ll.HAND_PAIR_CHAIN_MIN_LENGTH, ll.HAND_TRIO_CHAIN_MIN_LENGTH };

	if duplicate < 1 or duplicate > 3 then
		return;
	end

	local i = duplicate;
	while cardnum > 0 do
		if lastrank - 1 ~= ll.Card_Rank(array.cards[i]) then
			-- chain break
			if i >= chainlen[duplicate] then
				-- chain
				hand = ll.Hand_Create();
				hand.type = ll.Hand_Format(primal[duplicate], ll.HAND_KICKER_NONE, ll.HAND_CHAIN);
				for j = 1, i do
					ll.CardArray_PushBack(hand.cards, ll.CardArray_PopFront(array));
				end

				table.insert(handlist, 1, hand);
			else
				-- not a chain
				for j = 1, i/duplicate do
					hand = ll.Hand_Create();
					hand.type = ll.Hand_Format(primal[duplicate], ll.HAND_KICKER_NONE, ll.HAND_CHAIN_NONE);
					for k = 1, duplicate do
						ll.CardArray_PushBack(hand.cards, ll.CardArray_PopFront(array));
					end

					table.insert(handlist, 1, hand);
				end
			end

			if array.length == 0 then
				break;
			end

			lastrank = ll.Card_Rank(array.cards[1]);
			i = duplicate;
		else
			-- chain still intact
			lastrank = ll.Card_Rank(array.cards[i]);
			i = i + duplicate;
		end
	end

	local k = i - duplicate;				-- step back
	if k ~= 0 and k == array.length then	-- all chained up
		-- can chain up
		if k >= chainlen[duplicate] then
			hand = ll.Hand_Create();
			hand.type = ll.Hand_Format(primal[duplicate], ll.HAND_KICKER_NONE, ll.HAND_CHAIN);
			for j = 1, i - duplicate do
				ll.CardArray_PushBack(hand.cards, ll.CardArray_PopFront(array));
			end

			table.insert(handlist, 1, hand);
		else
			for j = 1, k/duplicate do
				hand = ll.Hand_Create();
				hand.type = ll.Hand_Format(primal[duplicate], ll.HAND_KICKER_NONE, ll.HAND_CHAIN_NONE);
				for i = 1, duplicate do
					ll.CardArray_PushBack(hand.cards, ll.CardArray_PopFront(array));	
				end

				table.insert(handlist, 1, hand);
			end
		end
	end
end

-- extract nuke, bomb and 2
function ll._HandList_ExtractNukeBomb2(handlist, array, count)
	local i = 0;
	local hand = nil;

	-- nuke
	if count[ll.CARD_RANK_r] ~= 0 and count[ll.CARD_RANK_R] ~= 0 then
		hand = ll.Hand_Create();
		hand.type = ll.Hand_Format(ll.HAND_PRIMAL_NUKE, ll.HAND_KICKER_NONE, ll.HAND_CHAIN_NONE);
		ll.CardArray_CopyRank(hand.cards, array, ll.CARD_RANK_R);
		ll.CardArray_CopyRank(hand.cards, array, ll.CARD_RANK_r);

		table.insert(handlist, 1, hand);

		count[ll.CARD_RANK_r] = 0;
		count[ll.CARD_RANK_R] = 0;

		ll.CardArray_RemoveRank(array, ll.CARD_RANK_r);
		ll.CardArray_RemoveRank(array, ll.CARD_RANK_R);
	end

	-- bomb
	for i = ll.CARD_RANK_2, ll.CARD_RANK_3, -1 do
		if count[i] == 4 then
			hand = ll.Hand_Create();
			hand.type = ll.Hand_Format(ll.HAND_PRIMAL_BOMB, ll.HAND_KICKER_NONE, ll.HAND_CHAIN_NONE);
			ll.CardArray_CopyRank(hand.cards, array, i);

			table.insert(handlist, 1, hand);

			count[i] = 0;
			ll.CardArray_RemoveRank(array, i);
		end
	end

	-- joker
	local jokerRank = 0;
	if count[ll.CARD_RANK_r] ~= 0 then
		jokerRank = ll.CARD_RANK_r;
	elseif count[ll.CARD_RANK_R] ~= 0 then
		jokerRank = ll.CARD_RANK_R;
	end

	if jokerRank ~= 0 then
		hand = ll.Hand_Create();
		ll.CardArray_CopyRank(hand.cards, array, jokerRank);
		hand.type = ll.HAND_PRIMAL_SOLO;
		table.insert(handlist, 1, hand);
		count[jokerRank] = 0;
		ll.CardArray_RemoveRank(array, jokerRank);
	end

	-- 2
	if count[ll.CARD_RANK_2] ~= 0 then
		hand = ll.Hand_Create();
		ll.CardArray_CopyRank(hand.cards, array, ll.CARD_RANK_2);

		if count[ll.CARD_RANK_2] == 1 then
			hand.type = ll.HAND_PRIMAL_SOLO;
		elseif count[ll.CARD_RANK_2] == 2 then
			hand.type = ll.HAND_PRIMAL_PAIR;
		elseif count[ll.CARD_RANK_2] == 3 then
			hand.type = ll.HAND_PRIMAL_TRIO;
		end

		count[ll.CARD_RANK_2] = 0;
		ll.CardArray_RemoveRank(array, ll.CARD_RANK_2);
		table.insert(handlist, 1, hand);
	end
end

function ll.HandList_StandardAnalyze(cards)
	local handlist = {};

	local arrsolo = ll.CardArray_Create();
	local arrpair = ll.CardArray_Create();
	local arrtrio = ll.CardArray_Create();
	local arrkicks = {};

	table.insert(arrkicks, arrsolo);
	table.insert(arrkicks, arrpair);
	table.insert(arrkicks, arrtrio);

	local array = ll.CardArray_Copy(cards);

	ll.CardArray_Sort(array);
	local count = ll.Hand_CountRank(array);

	-- nuke, bomb and 2
	ll._HandList_ExtractNukeBomb2(handlist, array, count);

	-- chains
	local i = 1;
	while i <= array.length do
		local c = count[ll.Card_Rank(array.cards[i])];
		if c ~= 0 then
			ll.CardArray_PushBackCards(arrkicks[c], array, i, c);
			i = i + c;
		else
			i = i + 1;
		end
	end
		
	-- chain
	ll._HandList_ExtractConsecutive(handlist, arrtrio, 3);
	ll._HandList_ExtractConsecutive(handlist, arrpair, 2);
	ll._HandList_ExtractConsecutive(handlist, arrsolo, 1);

	return handlist;
end

-----------------------------------------------------------------------------
-- hand evaluator
-----------------------------------------------------------------------------

function ll._HandList_CalculateConsecutive(array, duplicate)
	local hands = 0;
	if array.length == 0 then
		return hands;
	end

	local cardnum = array.length / duplicate;
	local lastrank = ll.Card_Rank(array.cards[1]);
	local primal = { ll.HAND_PRIMAL_SOLO, ll.HAND_PRIMAL_PAIR, ll.HAND_PRIMAL_TRIO };
	local chainlen = { ll.HAND_SOLO_CHAIN_MIN_LENGTH, ll.HAND_PAIR_CHAIN_MIN_LENGTH, ll.HAND_TRIO_CHAIN_MIN_LENGTH };

	if duplicate < 1 or duplicate > 3 then
		return hands;
	end

	local i = duplicate;
	while cardnum > 0 do
		if lastrank - 1 ~= ll.Card_Rank(array.cards[i]) then
			-- chain break
			if i >= chainlen[duplicate] then
				-- chain
				for j = 1, i do
					ll.CardArray_PopFront(array);
				end

				hands = hands + 1;
			else
				-- not a chain
				for j = 1, i/duplicate do
					for k = 1, duplicate do
						ll.CardArray_PopFront(array);
					end

					hands = hands + 1;
				end
			end

			if array.length == 0 then
				break;
			end

			lastrank = ll.Card_Rank(array.cards[1]);
			i = duplicate;
		else
			-- chain still intact
			lastrank = ll.Card_Rank(array.cards[i]);
			i = i + duplicate;
		end
	end

	local k = i - duplicate;				-- step back
	if k ~= 0 and k == array.length then	-- all chained up
		-- can chain up
		if k >= chainlen[duplicate] then
			for j = 1, i - duplicate do
				ll.CardArray_PopFront(array);
			end

			hands = hands + 1;
		else
			for j = 1, k/duplicate do
				for i = 1, duplicate do
					ll.CardArray_PopFront(array);
				end

				hands = hands + 1;
			end
		end
	end

	return hands;
end

function ll.HandList_StandardEvaluator(array)
	local hands = 0;
	local arrsolo = ll.CardArray_Create();
	local arrpair = ll.CardArray_Create();
	local arrtrio = ll.CardArray_Create();
	local arrkicks = {};

	table.insert(arrkicks, arrsolo);
	table.insert(arrkicks, arrpair);
	table.insert(arrkicks, arrtrio);

	local array = ll.CardArray_Copy(cards);

	ll.CardArray_Sort(array);
	local count = ll.Hand_CountRank(array);

	-- nuke
	if count[ll.CARD_RANK_R] ~= 0 and count[ll.CARD_RANK_R] ~= 0 then
		hands = hands + 1;
		count[ll.CARD_RANK_R] = 0;
		count[ll.CARD_RANK_r]  = 0;
	end

	-- bomb
	for i = ll.CARD_RANK_2, ll.CARD_RANK_3, -1 do
		if count[i] == 4 then
			hands = hands + 1;
			count[i] = 0;
		end
	end

	-- joker
	if count[ll.CARD_RANK_R] ~= 0 or count[ll.CARD_RANK_r] ~= 0 then
		hands = hands + 1;
		count[ll.CARD_RANK_R] = 0;
		count[ll.CARD_RANK_r]  = 0;
	end

	-- 2
	if count[ll.CARD_RANK_2] ~= 0 then
		hands = hands + 1;
		count[ll.CARD_RANK_2] = 0;
	end;

	-- chains
	local i = 1;
	while i <= array.length do
		local c = count[ll.Card_Rank(array.cards[i])];
		if c ~= 0 then
			ll.CardArray_PushBackCards(arrkicks[c], array, i, c);
			i = i + c;
		else
			i = i + 1;
		end
	end
		
	-- chain
	hands = hands + ll._HandList_CalculateConsecutive(arrtrio, 3);
	hands = hands + ll._HandList_CalculateConsecutive(arrpair, 2);
	hands = hands + ll._HandList_CalculateConsecutive(arrsolo, 1);

	return hands;
end

-----------------------------------------------------------------------------
-- advanced hand analyze
-----------------------------------------------------------------------------

function ll._HandList_SearchLongestConsecutive(handctx, hand, duplicate)
	-- context
	local rankstart = 0;
	local lastrank = 0;
	local primal = { ll.HAND_PRIMAL_SOLO, ll.HAND_PRIMAL_PAIR, ll.HAND_PRIMAL_TRIO };
	local chainlen = { ll.HAND_SOLO_CHAIN_MIN_LENGTH, ll.HAND_PAIR_CHAIN_MIN_LENGTH, ll.HAND_TRIO_CHAIN_MIN_LENGTH };
	local chain = ll.CardArray_Create();
	local count = handctx.count;
	local cards = handctx.cards;

	if duplicate < 1 or duplicate > 3 then
		return;
	end

	-- early break
	if cards.length < chainlen[duplicate] then
		return;
	end

	-- setup
	rankstart = 0;
	lastrank = ll.Card_Rank(cards.cards[1]);

	--
	-- i <= ll.CARD_RANK_2
	-- but count[CARD_RANK_2] must be 0
	-- for 2/bomb/nuke has been removed before calling this function
	--
	for i = ll.CARD_RANK_3, ll.CARD_RANK_2 do
		-- find start of a possible chain
		if rankstart == 0 then
			if count[i] >= duplicate then
				rankstart = i;
			end
		else
			-- chain break;
			if count[i] < duplicate then
				-- chain break, extract chain and set new possible start
				if ((i - rankstart) * duplicate) >= chainlen[duplicate] and (i - rankstart) > chain.length then
					-- valid chain, store rank in CardArray
					ll.CardArray_Clear(chain);
					for j = rankstart, i - 1 do
						ll.CardArray_PushBack(chain, j);
					end
				end

				rankstart = 0;
			end
		end
	end	

	-- convert rank array to card array
	if chain.length > 0 then
		local k = 0;
		for i = chain.length, 1, -1 do 
			lastrank = chain.cards[i];
			k = duplicate;
			for j = 1, cards.length do
				if ll.Card_Rank(cards.cards[j]) == lastrank then
					ll.CardArray_PushBack(hand.cards, cards.cards[j]);
					k = k - 1;

					if k == 0 then
						break;
					end
				end
			end
		end

		hand.type = ll.Hand_Format(primal[duplicate], ll.HAND_KICKER_NONE, ll.HAND_CHAIN);
	end
end

function ll._HandList_SearchPrimal(handctx, primal)
	local count = handctx.count;
	local primals = { ll.HAND_PRIMAL_SOLO, ll.HAND_PRIMAL_PAIR, ll.HAND_PRIMAL_TRIO };
	local rcards = handctx.rcards;
	local hand = nil;

	if primal > 3 or primal < 1 then
		return nil;
	end

	-- search count[rank] >= primal
	for i = 1, rcards.length do
		if count[ll.Card_Rank(rcards.cards[i])] >= primal then
			-- found
			hand = ll.Hand_Create();
			ll.CardArray_PushBackCards(hand.cards, rcards, i, primal);
			hand.type = ll.Hand_Format(primals[primal], ll.HAND_KICKER_NONE, ll.HAND_CHAIN_NONE);
			break;
		end
	end

	return hand;
end

ll._HAND_SEARCH_TYPES = 3;

--[[
-- pass a empty hand to start traverse
-- result stores in hand
-- return false when stop
--]]
function ll._HLAA_TraverseHands(handctx, begin, hand)
	local found = false;
	local i = begin;
	local primals = { 1, 2, 3 };
	-- solo chain, pair chain, trio chain, trio, pair, solo
	local searchers = { };
	table.insert(searchers, ll._HandList_SearchLongestConsecutive);
	table.insert(searchers, ll._HandList_SearchLongestConsecutive);
	table.insert(searchers, ll._HandList_SearchLongestConsecutive);

	if handctx.cards.length == 0 then
		return false, i;
	end

	if begin >= ll._HAND_SEARCH_TYPES then
		return false, i;
	end

	-- init search
	if hand.type == 0 then
		while i <= ll._HAND_SEARCH_TYPES and hand.type == 0 do
			searchers[i](handctx, hand, primals[i]);
			if hand.type ~= 0 then
				found = true;
				break;
			else
				i = i + 1;
			end
		end

		-- if found == 0, should PANIC
	else
		-- continue search via beat
		found = ll.HandList_SearchBeat(handctx.cards, hand, hand);
	end

	return found, i;
end

-- extract all chains or primal hands in handctx
function ll._HLAA_ExtractAllChains(handctx, hands)
	local found = false;
	local lastsearch = 1;
	local workinghand = ll.Hand_Create();
	local lasthand = ll.Hand_Create();

	-- init search
	found, lastsearch = ll._HLAA_TraverseHands(handctx, lastsearch, lasthand);

	while found do
		table.insert(hands, 1, ll.Hand_Copy(lasthand));
		workinghand = ll.Hand_Copy(lasthand);
		lasthand = ll.Hand_Create();

		repeat
			found, lastsearch = ll._HLAA_TraverseHands(handctx, lastsearch, workinghand);
			if found then
				local tmphand = ll.Hand_Copy(workinghand);
				table.insert(hands, 1, tmphand);
			end
		until not found;

		-- can't find any more hands, try to reduce chain length
		if lasthand.type ~= 0 then
			if lasthand.type == ll.Hand_Format(ll.HAND_PRIMAL_SOLO, ll.HAND_KICKER_NONE, ll.HAND_CHAIN) then
				if lasthand.cards.length > ll.HAND_SOLO_CHAIN_MIN_LENGTH then
					ll.CardArray_DropFront(lasthand.cards, 1);
					found = true;
				else
					lasthand.type = 0;
				end
			elseif lasthand.type == ll.Hand_Format(ll.HAND_PRIMAL_PAIR, ll.HAND_KICKER_NONE, ll.HAND_CHAIN) then
				if lasthand.card.lasthand > ll.HAND_PAIR_CHAIN_MIN_LENGTH then
					ll.CardArray_DropFront(lasthand.cards, 2);
					found = true;
				else
					lasthand.type = 0;
				end
			elseif lasthand.type == ll.Hand_Format(ll.HAND_PRIMAL_TRIO, ll.HAND_KICKER_NONE, ll.HAND_CHAIN) then
				if lasthand.card.lasthand > ll.HAND_TRIO_CHAIN_MIN_LENGTH then
					ll.CardArray_DropFront(lasthand.cards, 3);
					found = true;
				else
					lasthand.type = 0;
				end
			end
		end

		-- still can't found, loop through hand type for more
		if not found then
			lastsearch = lastsearch + 1;
			ll.Hand_Clear(lasthand);
			found, lastsearch = ll._HLAA_TraverseHands(handctx, lastsearch, lasthand);
		end
	end
end

-----------------------------------------------------------------------------
-- advanced search tree payload
-- 
-- hltree_payload
-- {
-- 		ctx 	: hand_ctx
-- 		hand 	: hand
--		weight	: number
-- }
-----------------------------------------------------------------------------

function ll._HLAA_CopyPayload(payload)
	local clone = {};
	clone.ctx = ll._HandContext_Copy(payload.ctx);
	clone.hand = ll.Hand_Copy(payload.hand);
	clone.weight = payload.weight;

	return clone;
end

function ll._HLAA_TreeAddHand(tree, hand)
	local oldpayload = tree.payload;
	local newpayload = {};
	newpayload.ctx = ll._HandContext_Copy(oldpayload.ctx);
	newpayload.hand = ll.Hand_Copy(hand);
	ll.CardArray_Subtract(newpayload.hand.cards, hand.cards);
	newpayload.ctx.rcards = ll.CardArray_Copy(newpayload.hand.cards);
	ll.CardArray_Reverse(newpayload.ctx.rcards);
	newpayload.ctx.count = ll.Hand_CountRank(newpayload.ctx.cards);
	newpayload.weight = oldpayload.weight + 1;

	local node = {};
	node.payload = newpayload;

	ll.Tree_AddChild(tree, node);

	return node;
end

-- search hand via least hands
function ll.HandList_AdvancedAnalyze(array)
	local handlist = {};
	local chains = {};
	local others = {};
	local tnode = nil;
	local temp = nil;
	local st = {};
	local shortest = nil;

	-- setup search context
	local ctx = ll.HandContext_Create(array);

	-- extract bombs and 2
	ll._HandList_ExtractNukeBomb2(handlist, ctx.cards, ctx.count);

	-- finish building beat_search_context
	ctx.rcards = ll.CardArray_Copy(ctx.cards);
	ll.CardArray_Reverse(ctx.rcards);

	-- magic goes here

	-- root
	local pload = {};
	pload.ctx = ll._HandContext_Copy(ctx);
	pload.hand = ll.Hand_Create();
	pload.weight = 0;

	local grandtree = {};
	grandtree.payload = pload;

	-- first expansion
	ll._HLAA_ExtractAllChains(ctx, chains);

	-- no chains, fall back to standard analyze
	if #chains == 0 then
		handlist = {};
		return ll.HandList_StandardAnalyze();
	end

	for k, v in pairs(chains) do
		tnode = ll._HLAA_TreeAddHand(grandtree, v);
		table.insert(st, 1, tnode);
	end

	chains = {};

	-- loop start
	while #st ~= 0 do
		-- pop stack
		temp = table.remove(st, 1);

		-- extract node data
		pload = temp.payload;

		-- expansion
		ll._HLAA_ExtractAllChains(pload.ctx, chains);

		if #chains ~= 0 then
			-- push new nodes
			for k, v in pairs(chains) do
				tnode = ll._HLAA_TreeAddHand(temp, v);
				table.insert(st, 1, tnode);
			end

			chains = {};
		end
	end

	-- tree construction complete
	st = ll.Tree_DumpToStack(grandtree);

	-- find shortest path
	-- FIXME
	while #st ~= 0 do
		-- pop stack
		temp = table.remove(st, 1);
		-- extract node data
		pload = temp.payload;

		-- calculate other hands weight
		pload.weight = pload.weight + ll.HandList_StandardEvaluator(pload.ctx.cards);

		if shortest == nil or pload.weight < shortest.payload.weight then
			shortest = temp;
		end

		others = {};
	end

	-- extract shortest node's other hands
	others = ll.HandList_StandardAnalyze(shortest.payload.ctx.cards);

	while shortest ~= nil and shortest.payload.weight ~= 0 do
		local handcpy = ll.Hand_Copy(shortest.payload.hand);
		table.insert(others, 1, handcpy);
		shortest = shortest.root;
	end

	ll.HandList_Concate(others, handlist);

	return others;
end

function ll.HandList_AdvancedEvaluator(array)
	local hl = ll.HandList_AdvancedAnalyze(array);
	return #hl;
end

function ll.Hand_Test()
-- TODO test current hand functions
    local cards2 = ll.CardArray_Create();
    ll.CardArray_PushBack(cards2, 0x14);
    ll.CardArray_PushBack(cards2, 0x24);
    ll.CardArray_PushBack(cards2, 0x15);
    ll.CardArray_PushBack(cards2, 0x25);
    ll.CardArray_PushBack(cards2, 0x35);
    ll.CardArray_PushBack(cards2, 0x16);
    ll.CardArray_PushBack(cards2, 0x26);
    ll.CardArray_PushBack(cards2, 0x36);
    ll.CardArray_PushBack(cards2, 0x17);
    ll.CardArray_PushBack(cards2, 0x27);
    ll.CardArray_PushBack(cards2, 0x37);
    ll.CardArray_PushBack(cards2, 0x47);
    ll.CardArray_PushBack(cards2, 0x28);
    ll.CardArray_PushBack(cards2, 0x38);
    ll.CardArray_PushBack(cards2, 0x29);
    ll.CardArray_PushBack(cards2, 0x39);
    
    local hl = ll.HandList_AdvancedAnalyze(cards2);
    for k, v in pairs(hl) do
        print("\n" .. ll.Hand_ToString(v));
    end
end