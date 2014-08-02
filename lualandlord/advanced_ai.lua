require "lualandlord/hand"

function ll.AdvancedAI_GetReady(player, game)
	ll.CardArray_Sort(player.cards);
	player.record = ll.CardArray_Copy(player.cards);
	player.handlist = ll.HandList_AdvancedAnalyze(player.cards);

	return 0;
end

function ll.AdvancedAI_Beat(player, game)
		-- HandList_SearchBeats can search for beat in loop mode
	-- but we just simply find a beat here

	local canbeat = false;
	local i = 0;
	local tobeat = nil;
	local beat = {};
	local prevplayer = nil;
	local teammate = nil;
	local landlord = nil;

	tobeat = game.lastHand;

	canbeat = ll.HandList_BestBeat(player.cards, tobeat, beat);

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

		if canbeat and (teammate.cards.length < player.cards.length and teammate.cards.length < 4) then
			canbeat = false;
		end
	end

	if canbeat then
		ll.CardArray_Subtract(player.cards, beat.cards);
		player.handlist = ll.HandList_AdvancedAnalyze(player.cards);
		tobeat.type = beat.type;
		tobeat.cards = ll.CardArray_Copy(beat.cards);
	end

	return canbeat;
end
