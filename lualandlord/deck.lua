-----------------------------------------------------------------------------
-- deck.lua
-- Landlord
--
-- Create by Master.G on 14-7-23.
-- Copyright (c) 2014 MED. All rights reserved.
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
-- deck definition and operations
-----------------------------------------------------------------------------

require "lualandlord/llinternal";
require "lualandlord/card";

-----------------------------------------------------------------------------
-- deck 
-- {
-- 		cards : CardArray
-- 		used  : CardArray
-- }
-----------------------------------------------------------------------------

function ll.shuffle(arr, randctx)
	local j = 0;
	for i = #arr, 1, -1 do
		if randctx ~= nil then
			j = ll.Random_Range(randctx, 1, #arr);
		else
			j = math.random(1, #arr);
		end

		local tmp = arr[j];
		arr[j] = arr[i];
		arr[i] = tmp;
	end
end

function ll.Deck_Create()
	local deck = {};
	deck.cards = ll.CardArray_CreateWithDeck();
	deck.used = ll.CardArray_Create();

	return deck;
end

function ll.Deck_Shuffle(deck, randctx)
	ll.shuffle(deck.cards.cards, randctx);
end

function ll.Deck_Reset(deck)
	ll.CardArray_Reset(deck.cards);
	ll.CardArray_Clear(deck.used);
end

function ll.Deck_DealSingle(deck)
	return ll.CardArray_PopFront(deck.cards);
end

function ll.Deck_RecycleSingle(deck, card)
	ll.CardArray_PushBack(deck.used, card);
end

function ll.Deck_Deal(deck, count)
	local array = ll.CardArray_Create();
	for i = 1, count do
		ll.CardArray_PushBack(array, ll.Deck_DealSingle(deck));
	end

	return array;
end

function ll.Deck_Recycle(deck, array)
	for i = 1, array.length do
		ll.Deck_RecycleSingle(deck, ll.CardArray_PopBack(array));
	end
end
