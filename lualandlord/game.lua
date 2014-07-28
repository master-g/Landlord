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
	x = x % ll.GAME_PLAYERS + 1;
	return x;
end