-----------------------------------------------------------------------------
-- card.lua
-- Landlord
--
-- Create by Master.G on 14-7-22.
-- Copyright (c) 2014 MED. All rights reserved.
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
-- card definition and operations
-----------------------------------------------------------------------------

require "lualandlord/llinternal";

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
	if card == nil then
		return 0;
	else
		return ll.BIT.band(card, 0x0F);
	end
end

function ll.Card_Suit(card)
	if card == nil then
		return 0;
	else
		return ll.BIT.band(card, 0x0F);
	end
end

function ll.Card_Red(card)
	if ll.Card_Suit(card) == ll.CARD_SUIT_HEART or ll.Card_Suit(card) == ll.CARD_SUIT_CLUB then
		return true;
	else
		return false;
	end
end

function ll.Card_Make(suit, rank)
	return ll.BIT.bor(suit, rank);
end

function ll.Card_ToString(card)
	local str = "";
	local rank = ll.Card_Rank(card);
	local suit = ll.Card_Suit(card);

	if suit == ll.CARD_SUIT_DIAMOND then
		--str = string.char(0xE2, 0x99, 0xA6);
		str = "d";
	elseif suit == ll.CARD_SUIT_CLUB then
		--str = string.char(0xE2, 0x99, 0xA3);
		str = "c";
	elseif suit == ll.CARD_SUIT_HEART then
		--str = string.char(0xE2, 0x99, 0xA5);
		str = "h";
	elseif suit == ll.CARD_SUIT_SPADE then
		--str = string.char(0xE2, 0x99, 0xA0);
		str = "s";
	end

	local strRank = { '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A', '2', 'r', 'R' };
	str = str .. strRank[rank];

	return str;
end	

-----------------------------------------------------------------------------
-- card array
-- {
-- 		length : number
-- 		cards : {}
-- }
-----------------------------------------------------------------------------

ll.CARD_ARRAY_PRESET_LENGTH = ll.CARD_SET_LENGTH;

ll.CardArray = {};

-- create a empty array
function ll.CardArray_Create()
	local cardarray = {};
	cardarray.length = 0;
	cardarray.cards = {};

	return cardarray;
end

-- create a card array with full deck
function ll.CardArray_CreateWithDeck()
	local cardarray = {};
	ll.CardArray_Reset(cardarray);

	return cardarray;
end

-- create a card array from string
function ll.CardArray_CreateWithString(str)
	local cardarray = ll.CardArray_Create();

	for i = 1, #str do
	    local c = str:sub(i,i)
	    local card = 0;

	    if c:byte() == 0xA6 then
	    	card = ll.BIT.bor(card, ll.CARD_SUIT_DIAMOND);
	    elseif c:byte() == 0xA3 then
	    	card = ll.BIT.bor(card, ll.CARD_SUIT_CLUB);
    	elseif c:byte() == 0xA5 then
    		card = ll.BIT.bor(card, ll.CARD_SUIT_HEART);
    	elseif c:byte() == 0xA0 then
    		card = ll.BIT.bor(card, ll.CARD_SUIT_SPADE);
    	elseif c == "T" then
    		card = ll.BIT.bor(card, ll.CARD_RANK_T);
    	elseif c == "J" then
    		card = ll.BIT.bor(card, ll.CARD_RANK_J);
    	elseif c == "Q" then
    		card = ll.BIT.bor(card, ll.CARD_RANK_Q);
    	elseif c == "K" then
    		card = ll.BIT.bor(card, ll.CARD_RANK_K);
    	elseif c == "A" then
    		card = ll.BIT.bor(card, ll.CARD_RANK_A);
    	elseif c == "2" then
    		card = ll.BIT.bor(card, ll.CARD_RANK_2);
    	elseif c == "r" then
    		card = ll.BIT.bor(card, ll.CARD_RANK_r);
    	elseif c == "R" then
    		card = ll.BIT.bor(card, ll.CARD_RANK_R);
    	end

    	if c:byte() <= string.byte("9") and c:byte() >= string.byte("3") then
    		card = ll.BIT.bor(card, ll.CARD_RANK_3 + c:byte() - string.byte("3"));
    	end

    	if ll.Card_Rank(card) ~= 0 and Card_Suit(card) ~= 0 then
    		ll.CardArray_PushBack(cardarray, card);
    		card = 0;
    	end
	end

	return cardarray;
end

-- get first card from array
function ll.CardArray_GetFront(cardarray)
	for k, v in pairs(cardarray.cards) do
		return v;
	end
end

-- get last card from array
function ll.CardArray_GetBack(cardarray)
	return cardarray.cards[cardarray.length];
end

-- copy card array
function ll.CardArray_Copy(cardarray)
	local clone = {};
	clone.length = cardarray.length;
	clone.cards = {};
	for k, v in pairs(cardarray.cards) do
		table.insert(clone.cards, v);
	end

	return clone;
end

-- clear card array
function ll.CardArray_Clear(cardarray)
	cardarray.length = 0;
	cardarray.cards = {};
end

-- check if there is no card in the array
function ll.CardArray_IsEmpty(cardarray)
	if cardarray.length == 0 then
		return true;
	else
		return false;
	end
end

-- check if the array is full
function ll.CardArray_IsFull(array)
	if array.length >= ll.CARD_ARRAY_PRESET_LENGTH then
		return true;
	else
		return false;
	end
end

-- return the capacity of the array
function ll.CardArray_Capacity(cardarray)
	return ll.CARD_ARRAY_PRESET_LENGTH - cardarray.length;
end

-- reset the card array with 54 poker cards
function ll.CardArray_Reset(cardarray)
	cardarray.length = ll.CARD_ARRAY_PRESET_LENGTH;
	cardarray.cards = 
	{
		0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
    	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,
    	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,
    	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D,
    	0x1E, 0x2F
	};
end

-- concate tail to head
function ll.CardArray_Concate(head, tail)
	local length = 0;
	local slot = 0;
	slot = ll.CARD_ARRAY_PRESET_LENGTH - head.length;

	if not ll.CardArray_IsEmpty(tail) and slot > 0 then
		if slot >= tail.length then
			length = tail.length;
		else
			length = slot;
		end

		for i = 1, length do
			table.insert(head.cards, tail.cards[i]);
			head.length = head.length + 1;
		end
	else
		length = 0;
	end

	return length;
end

-- subtract two card arrays
function ll.CardArray_Subtract(from, sub)
	local i = 0;
	local card = 0;
	local result = {};

	for fk, fv in pairs(from.cards) do
		card = fv;
		for sk, sv in pairs(sub.cards) do
			if fv == sv then
				card = 0;
				break;
			end
		end

		if card ~= 0 then
			i = i + 1;
			table.insert(result, card);
		end
	end

	from.length = i;
	from.cards = result;
end

-- check for identity
function ll.CardArray_IsIdentity(array1, array2)
	if array1.length ~= array2.length then
		return false;
	else
		local c1 = ll.CardArray_Copy(array1);
		local c2 = ll.CardArray_Copy(array2);
		ll.CardArray_Sort(c1);
		ll.CardArray_Sort(c2);

		for i = 1, c1.length do
			if c1.cards[i] ~= c2.cards[i] then
				return false;
			end
		end

		return true;
	end
end

-- check if array contains segment
function ll.CardArray_Contains(array, segment)
	if array.length == 0 or segment.length == 0 or array.length < segment.length then
		return false;
	end

	local c = ll.CardArray_Copy(segment);
	for ka, va in pairs(array.cards) do
		for ks, vs in pairs(c.cards) do
			if va == vs then
				c[ks] = nil;
			end
		end
	end

	for i = 1, c.length do
		if c.cards[i] ~= nil then
			return false;
		end
	end

	return true;
end

-- push a card to the back of the array
function ll.CardArray_PushBack(array, card)
	if not ll.CardArray_IsFull(array) then
		table.insert(array.cards, card);
		array.length = array.length + 1;
	end
end

-- push a card to the front of the array
function ll.CardArray_PushFront(array, card)
	if ll.CardArray_IsFull(array) then
		return;
	end

	local newarray = {};
	table.insert(newarray, card);
	for k, v in pairs(array.cards) do
		table.insert(newarray, v);
	end

	array.length = array.length + 1;
	array.cards = newarray;
end

-- remove a card from the back of the array
function ll.CardArray_PopBack(array)
	if ll.CardArray_IsEmpty(array) then
		return nil;
	end

	local card = table.remove(array.cards, array.length);
	array.length = array.length - 1;

	return card;
end

-- remove a card from the front of the array
function ll.CardArray_PopFront(array)
	if ll.CardArray_IsEmpty(array) then
		return nil;
	end

	local card = table.remove(array.cards, 1);
	array.length = array.length - 1;

	return card;
end

-- drop multiple cards from the back of the array
function ll.CardArray_DropBack(array, count)
	local drop = 0;
	if array.length >= count then
		drop = count;
	else
		drop = array.length;
	end

	for i = 1, drop do
		table.remove(array.cards, array.length);
		array.length = array.length - 1;
	end

	return drop;
end

-- drop multiple cards from the front of the array
function ll.CardArray_DropFront(array, count)
	local drop = 0;
	if array.length >= count then
		drop = count;
	else
		drop = array.length;
	end

	for i = 1, drop do
		table.remove(array.cards, 1);
		array.length = array.length - 1;
	end

	return drop;
end

-- insert a card into array
function ll.CardArray_Insert(array, card, where)
	if ll.CardArray_IsFull(array) then
		return;
	end

	table.insert(array.cards, where, card);
	array.length = array.length + 1;
end

-- remove a card from array
function ll.CardArray_Remove(array, where)
	table.remove(array.cards, where);
	array.length = array.length - 1;
end

-- remove specific card from array
function ll.CardArray_RemoveCard(array, card)
	local remove = 0;
	local newarray = {};
	for k, v in pairs(array.cards) do
		if v ~= card then
			table.insert(newarray, v);
		else
			array.length = array.length - 1;
			remove = remove + 1;
		end
	end

	array.cards = newarray;

	return remove;
end

-- push cards from specific position of array 'from' to the back of array
function ll.CardArray_PushBackCards(array, from, where, count)
	for i = 0, count - 1 do
		ll.CardArray_PushBack(array, from.cards[where + i]);
	end
end

-- copy specific rank cards into another array
function ll.CardArray_CopyRank(dst, src, rank)
	local cards = 0;

	for k, v in pairs(src.cards) do
		if ll.Card_Rank(v) == rank then
			ll.CardArray_PushBack(dst, v);
			cards = cards + 1;
		end
	end

	return cards;
end

-- remove specific rank cards from array
function ll.CardArray_RemoveRank(array, rank)
	local newarray = {};
	local newlength = 0;
	for k, v in pairs(array.cards) do
		if ll.Card_Rank(v) ~= rank then
			table.insert(newarray, v);
			newlength = newlength + 1;
		end
	end

	array.length = newlength;
	array.cards = newarray;
end

-- sort cards
function ll.CardArray_Sort(array, sortfunc)
	if sortfunc ~= nil then
		table.sort(array.cards, sortfunc);
	else
		table.sort(array.cards, 
			function(a, b)
				local ra = ll.BIT.band(a, 0xF0) / 16 + ll.BIT.band(a, 0x0F) * 16;
				local rb = ll.BIT.band(b, 0xF0) / 16 + ll.BIT.band(b, 0x0F) * 16;

				return ra > rb;
			end
		);
	end
end

-- reverse cards in the array
function ll.CardArray_Reverse(array)
	local newarray = {};
	for i = array.length, 1, -1 do
		table.insert(newarray, array.cards[i]);
	end

	array.cards = newarray;
end

function ll.CardArray_ToString(array)
	local str = "";
	for k, v in pairs(array.cards) do
		str = str .. ll.Card_ToString(v);
	end

	return str;
end
