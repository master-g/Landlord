-----------------------------------------------------------------------------
-- llinternal.lua
-- Landlord
--
-- Create by Master.G on 14-7-22.
-- Copyright (c) 2014 MED. All rights reserved.
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
-- internal
-----------------------------------------------------------------------------

ll = {};
ll.BIT = require "lualandlord/numberlua"

-----------------------------------------------------------------------------
-- random
-----------------------------------------------------------------------------

function ll.Random_Create(seed)
	local s = seed or 0;
	local ctx = {};
	ctx.seed = s;
	ctx.sequence = 0;

	return ctx;
end

function ll.Random_Real(ctx)
	local r = 0;
	math.randomseed(ctx.seed);
	for i = 1, ctx.sequence + 1 do
		r = math.random();
	end

	ctx.sequence = ctx.sequence + 1;

	return r;
end

function ll.Random_Int32(ctx)
	local r = 0;
	math.randomseed(ctx.seed);
	for i = 1, ctx.sequence + 1 do
		r = math.random(-2147483648, 2147483647);
	end

	ctx.sequence = ctx.sequence + 1;

	return r;
end

function ll.Random_UInt32(ctx)
	local r = 0;
	math.randomseed(ctx.seed);
	for i = 1, ctx.sequence + 1 do
		r = math.random(0, 4294967295);
	end

	ctx.sequence = ctx.sequence + 1;

	return r;
end

function ll.Random_Range(ctx, floor, ceiling)
	local r = 0;
	math.randomseed(ctx.seed);
	for i = 1, ctx.sequence + 1 do
		r = math.random(floor, ceiling);
	end

	ctx.sequence = ctx.sequence + 1;

	return r;
end
