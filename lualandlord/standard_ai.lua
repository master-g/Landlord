require "lualandlord/hand"

function ll.StandardAI_GetReady(player, game)
	ll.CardArray_Sort(player.cards);
	player.record = ll.CardArray_Copy(player.cards);
	player.handlist = ll.HandList_StandardAnalyze(player.cards);

	return 0;
end

function ll.StandardAI_Bid(player, game)
	local shouldbid = 0;
	local handlistlen = 0;
	ll.CardArray_Sort(player.cards);
	player.handlist = ll.HandList_StandardAnalyze(player.cards);
	handlistlen = #player.handlist;

	if handlistlen > 12 then
		shouldbid = 0;
	elseif handlistlen == 12 then
		shouldbid = 1;
	elseif handlistlen == 11 then
		shouldbid = 2;
	elseif handlistlen < 11 then
		shouldbid = 3;
	end

	if shouldbid > game.bid then
		return shouldbid;
	else
		return 0;
	end
end

function ll.StandardAI_Play(player, game)
	local need = 0;
	local searchprimal = 0;
	local kicker = 0;

	local hand = game.lastHand;

	repeat
		-- empty hands
		if #player.handlist == 0 then
			hand.type = 0;
			break;
		end

		-- last hand
		if #player.handlist == 1 then
			local last = player.handlist[1];
			hand.type = last.type;
			hand.cards = ll.CardArray_Copy(last.cards);
			player.handlist = {};
			break;
		end

		-- try to find longest hand combination
		local node = ll.HandList_Find(player.handlist, ll.Hand_Format(ll.HAND_PRIMAL_TRIO, ll.HAND_KICKER_NONE, ll.HAND_CHAIN));
		if node ~= nil then
			-- copy hand
			hand.type = node.type;
			hand.cards = ll.CardArray_Copy(node.cards);

			-- how many kickers do we need
			need = node.cards.length / 3;

			-- remove hand node from list
			ll.HandList_Remove(player.handlist, node);

			-- count solo and pair number
			local countpair = 0;
			local countsolo = 0;
			for k, v in pairs(player.handlist) do
				if v.type == ll.HAND_PRIMAL_PAIR then
					countpair = countpair + 1;
				elseif v.type == ll.HAND_PRIMAL_SOLO then
					countsolo = countsolo + 1;
				end
			end

			-- trio-pair-chain then trio-solo-chain
			if countsolo < need and countpair < need then
				break;
			elseif countpair >= need then
				searchprimal = ll.HAND_PRIMAL_PAIR;
				kicker = ll.HAND_KICKER_PAIR;
			else
				searchprimal = ll.HAND_PRIMAL_SOLO;
				kicker = ll.HAND_KICKER_SOLO;
			end

			-- detach pairs from list
			for k, v in pairs(player.handlist) do
				if need <= 0 then
					break;
				end

				local rest = {};
				if v.type == searchprimal then
					-- copy cards
					ll.CardArray_Concate(hand.cards, v.cards);
					need = need - 1;
				else
					table.insert(rest, v);
				end
			end

			player.handlist = rest;

			hand.type = ll.Hand_SetKicker(hand.type, kicker);
			break;
		end

		-- pair chain
		node = ll.HandList_Find(player.handlist, ll.Hand_Format(ll.HAND_PRIMAL_PAIR, ll.HAND_KICKER_NONE, ll.HAND_CHAIN));
		-- solo chain
		if node == nil then
			node = ll.HandList_Find(player.handlist, ll.Hand_Format(ll.HAND_PRIMAL_SOLO, ll.HAND_KICKER_NONE, ll.HAND_CHAIN));
		end

		if node ~= nil then
			hand.type = node.type;
			hand.cards = ll.CardArray_Copy(node.cards);
			ll.HandList_Remove(player.handlist, node);
			break;
		end

		-- trio
		node = ll.HandList_Find(player.handlist, ll.HAND_PRIMAL_TRIO);
		if node ~= nil and ll.Card_Rank(node.cards.cards[1]) ~= ll.CARD_RANK_2 then
			hand.type = node.type;
			hand.cards = ll.CardArray_Copy(node.cards);

			ll.HandList_Remove(player.handlist, node);

			-- pair
			node = ll.HandList_Find(player.handlist, ll.HAND_PRIMAL_PAIR);
			if node ~= nil and ll.Card_Rank(node.cards.cards[1]) ~= ll.CARD_RANK_2 then
				kicker = ll.HAND_KICKER_PAIR;
			else
				-- solo
				node = ll.HandList_Find(player.handlist, ll.HAND_PRIMAL_SOLO);
				if node ~= nil and ll.Card_Rank(node.cards.cards[1]) ~= ll.CARD_RANK_2 then
					kicker = ll.HAND_KICKER_SOLO;
				end
			end

			if node ~= nil then
				ll.CardArray_Concate(hand.cards, node.cards);
				hand.type = ll.Hand_SetKicker(hand.type, kicker);
				ll.HandList_Remove(player.handlist, node);
				break;
			end
		end

		-- pair
		node = ll.HandList_Find(player.handlist, ll.HAND_PRIMAL_PAIR);
		if node ~= nil and ll.Card_Rank(node.cards.cards[1]) ~= ll.CARD_RANK_2 then
			hand.type = node.type;
			hand.cards = ll.CardArray_Copy(node.cards);
			ll.HandList_Remove(player.handlist, node);
			break;
		end

		-- just play
		node = player.handlist[1];
		hand.type = node.type;
		hand.cards = ll.CardArray_Copy(node.cards);
		ll.HandList_Remove(player.handlist, node);

	until true;

	ll.CardArray_Subtract(player.cards, hand.cards);

	return 0;
end

function ll.StandardAI_Beat(player, game)
	-- HandList_SearchBeats can search for beat in loop mode
	-- but we just simply find a beat here

	local canbeat = false;
	local i = 0;
	local tobeat = nil;
	local beat = ll.Hand_Create();
	local prevplayer = nil;
	local teammate = nil;
	local landlord = nil;

	tobeat = game.lastHand;

	canbeat = ll.HandList_SearchBeat(player.cards, tobeat, beat);

	-- peasant teamwork
	prevplayer = game.players[game.lastplay];
	if canbeat and 
		(player.identity == ll.PlayerIdentity_Peasant) and
		(prevplayer.identity == ll.PlayerIdentity_Peasant) then

		-- find teammate and landlord
		for k, v in pairs(game.players) do
			if (v.identity == ll.PlayerIdentity_Landlord) then
				landlord = v;
			end

			if (v.identity == ll.PlayerIdentity_Peasant) and (v ~= player) then
				teammate = v;
			end
		end

		-- don't bomb/nuke teammate
		if canbeat and ((beat.type == ll.HAND_PRIMAL_NUKE) or (beat.type == ll.HAND_PRIMAL_BOMB)) then
			canbeat = false;
		end

		if canbeat and teammate.cards.length < player.cards.length then
			canbeat = false;
		end
	end

	if canbeat then
		ll.CardArray_Subtract(player.cards, beat.cards);
		player.handlist = ll.HandList_StandardAnalyze(player.cards);
		tobeat.type = beat.type;
		tobeat.cards = ll.CardArray_Copy(beat.cards);
	end

	return canbeat;
end
