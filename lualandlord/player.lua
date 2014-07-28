require "lualandlord/hand";
require "lualandlord/standard_ai";
require "lualandlord/advanced_ai";

ll.PlayerEvent_GetReady 	= 1;
ll.PlayerEvent_Bid 			= 2;
ll.PlayerEvent_Start		= 3;
ll.PlayerEvent_Play			= 4;
ll.PlayerEvent_Beat 		= 5;

ll.PlayerIdentity_Peasant	= 1;
ll.PlayerIdentity_Landlord	= 2;

ll.PlayerBid_Abstain		= 1;
ll.PlayerBid_Bid			= 2;


-----------------------------------------------------------------------------
-- player
--
-- {
-- 		cards			: CardArray
-- 		record			: CardArray
--		handlist		: {}
--		identity 		: number
-- 		seatId 			: number
-- 		bid 			: number
-- 		eventHandlers	: {}
-- }
-----------------------------------------------------------------------------

-- create a empty player
function ll.Player_Create()
	local player = {};
	player.cards = ll.CardArray_Create();
	player.record = ll.CardArray_Create();
	player.handlist = {};
	player.seatId = 0;
	player.bid = 0;
	player.eventHandlers = {};

	return player;
end

-- create a standard AI player
function ll.Player_CreateStandardAI()
	local player = ll.Player_Create();

	player.eventHandlers[ll.PlayerEvent_GetReady] 	= ll.StandardAI_GetReady;
	player.eventHandlers[ll.PlayerEvent_Bid] 		= ll.StandardAI_Bid;
	player.eventHandlers[ll.PlayerEvent_Play] 		= ll.StandardAI_Play;
	player.eventHandlers[ll.PlayerEvent_Beat]		= ll.StandardAI_Beat;

	return player;
end

-- create a advanced AI player
function ll.Player_CreateAdvancedAI()
	local player = ll.Player_Create();

	player.eventHandlers[ll.PlayerEvent_GetReady] 	= ll.AdvancedAI_GetReady;
	player.eventHandlers[ll.PlayerEvent_Bid] 		= ll.StandardAI_Bid;
	player.eventHandlers[ll.PlayerEvent_Play] 		= ll.StandardAI_Play;
	player.eventHandlers[ll.PlayerEvent_Beat]		= ll.AdvancedAI_Beat;

	return player;
end

-- clear a player's context
function ll.Player_Clear(player)
	player.handlist = {};
	player.identity = ll.PlayerIdentity_Peasant;
	ll.CardArray_Clear(player.cards);
	ll.CardArray_Clear(player.record);
end

-- handle event
function ll.Player_HandleEvent(player, event, ctx)
	return player.eventHandlers[event](player, ctx);
end

