require "lualandlord/hand";
require "lualandlord/player";

-----------------------------------------------------------------------------
-- constants 
-----------------------------------------------------------------------------

ll.GAME_PLAYERS 	= 3;
ll.GAME_HAND_CARDS 	= 17;
ll.GAME_REST_CARDS	= 3;
ll.GAME_BID_1		= 1;
ll.GAME_BID_2 		= 2;
ll.GAME_BID_3 		= 3;

function ll.IncPlayerIndex(i)
	i = i % ll.GAME_PLAYERS + 1;
	return i;
end

function ll.Game_GetCurrentPlayer(game)
	return game.players[game.playerIndex];
end

function ll.Game_IncPlayerIndex(game)
	game.playerIndex = ll.IncPlayerIndex(game.playerIndex);
end

ll.GameStatus_Halt 		= 1;
ll.GameStatus_Bid 		= 2;
ll.GameStatus_Ready		= 3;
ll.GameStatus_Pause 	= 4;
ll.GameStatus_Over 		= 5;

ll.Phase_Play 			= 1;
ll.Phase_Query 			= 2;
ll.Phase_Pass 			= 3;

-----------------------------------------------------------------------------
-- game
-----------------------------------------------------------------------------

function ll.Game_Create()
	local game = {};

	-- players
	game.players = {};
	for i = 1, ll.GAME_PLAYERS do 
		local player = ll.Player_Create();
		table.insert(game.players, player);
		player.seatId = i;
		player.identity = ll.PlayerIdentity_Peasant;
		player.handlist = {};
	end

	-- deck
	game.deck = ll.Deck_Create();

	-- random context
	game.mt = ll.Random_Create();

	-- last played hand
	game.lastHand = ll.Hand_Create();

	-- card record
	game.cardRecord = ll.CardArray_Create();

	-- kitty cards
	game.kittyCards = ll.CardArray_Create();

	-- current bid
	game.bid = 0;

	-- for the highest bidder !
	game.highestBidder = 0;

	-- current player index
	game.playerIndex = 0;

	-- landlord index
	game.landlord = 0;

	-- last played player index
	game.lastplay = 0;

	-- who win the last game
	game.winner = 0;

	-- game status
	game.status = 0;

	-- game phase
	game.phase = 0;

	return game;
end

function ll.Game_Clear(game)
	for idx, player in pairs(game.players) do 
		ll.Player_Clear(player);
	end
end

function ll.Game_Reset(game)
	for k, player in pairs(game.players) do
		ll.Player_SetupStandardAI(player);
		ll.Player_Clear(player);
	end

	game.bid = 0;
	game.playerIndex = 0;
	game.landlord = 0;
	game.lastplay = 0;
	game.winner = 0;
	game.status = 0;
	game.phase = 0;

	ll.Hand_Clear(game.lastHand);
	ll.Deck_Reset(game.deck);
	ll.Deck_Shuffle(game.deck, game.random);
	ll.CardArray_Clear(game.kittyCards);
	ll.CardArray_Clear(game.cardRecord);
end

function ll.Game_Play(game, seed)
	local beat = false;
	local bid = 0;

	ll.Random_SetSeed(game.mt, seed);

	-- bid
	game.status = ll.GameStatus_Bid;
	game.bid = 0;
	game.highestBidder = 0;

	while game.status == ll.GameStatus_Bid do
		game.playerIndex = ll.Random_Range(game.mt, 1, ll.GAME_PLAYERS);
		for i = 1, ll.GAME_PLAYERS do
			local player = ll.Game_GetCurrentPlayer(game);
			player.cards = ll.Deck_Deal(game.deck, ll.GAME_HAND_CARDS);
			bid = ll.Player_HandleEvent(player, ll.PlayerEvent_Bid, game);
			if bid > game.bid then
				print(string.format("Player ---- %d ---- bid for %d", game.playerIndex, bid));
				game.highestBidder = game.playerIndex;
				game.bid = bid;
			end

			ll.Game_IncPlayerIndex(game);
		end

		-- check if bid stage is finished
		if game.bid == 0 then
			ll.Deck_Reset(game.deck);
			ll.Deck_Shuffle(game.deck, game.mt);
		else
			-- setup landlord, game start!
			game.landlord = game.highestBidder;
			game.players[game.landlord].identity = ll.PlayerIdentity_Landlord;
			game.playerIndex = game.landlord;
			game.phase = ll.Phase_Play;
			game.kittyCards = ll.Deck_Deal(game.deck, ll.GAME_REST_CARDS);
			ll.CardArray_Concate(game.players[game.landlord].cards, game.kittyCards);
			game.status = ll.GameStatus_Ready;
		end
	end

	for k, player in pairs(game.players) do 
		ll.Player_HandleEvent(player, ll.PlayerEvent_GetReady, game);
		print(string.format("player : %d ", player.seatId) .. ll.CardArray_ToString(player.cards));
	end

	-- game play

	while game.status ~= ll.GameStatus_Over do 

		if game.phase == ll.Phase_Play then
			ll.Player_HandleEvent(ll.Game_GetCurrentPlayer(game), ll.PlayerEvent_Play, game);
			game.lastplay = game.playerIndex;
			game.phase = ll.Phase_Query;

			ll.CardArray_Concate(game.cardRecord, game.lastHand.cards);

			print(string.format("Player ---- %d ---- played", game.playerIndex));
			print(ll.Hand_ToString(game.lastHand));
		elseif game.phase == ll.Phase_Query or game.phase == ll.Phase_Pass then
			beat = ll.Player_HandleEvent(ll.Game_GetCurrentPlayer(game), ll.PlayerEvent_Beat, game);
			-- has beat in this phase
			if not beat then
				-- two player pass
				if game.phase == ll.Phase_Pass then
					game.phase = ll.Phase_Play;
				else
					game.phase = ll.Phase_Pass;
				end

				print(string.format("Player ---- %d ---- passed", game.playerIndex));
			else
				game.lastplay = game.playerIndex;
				game.phase = ll.Phase_Query;
				ll.CardArray_Concate(game.cardRecord, game.lastHand.cards);

				print(string.format("Player ---- %d ---- beat", game.playerIndex));
				print(ll.Hand_ToString(game.lastHand));
			end
		end

		ll.Game_IncPlayerIndex(game);

		-- check if there is player win
		for k, player in pairs(game.players) do 
			if player.cards.length == 0 then
				game.status = ll.GameStatus_Over;
				game.winner = player.seatId;

				print(string.format("Player ++++ %d ++++ wins !", player.seatId));
				break;
			end
		end
	end
end

function ll.Game_Test()
	local peasantwon = 0;
	local landlordwon = 0;
	local i = 1;

	local game = ll.Game_Create();

	print(string.format("start at %d\n", os.time()));

	ll.Game_Reset(game);

	for i = 100000, 100001 do
		ll.Game_Play(game, i);

		if game.winner == game.landlord then
			landlordwon = landlordwon + 1;
		else 
			peasantwon = peasantwon + 1;
		end

		ll.Game_Reset(game);
	end

	print(string.format("peasants : %d", peasantwon));
	print(string.format("landlord : %d", landlordwon));

	print(string.format("ended at %d", os.time()));
end
