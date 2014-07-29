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

function ll.Random_SetSeed(ctx, seed)
	--[[
	ctx.seed = seed;
	ctx.sequence = 0;
	--]]

	math.randomseed(seed);
end

function ll.Random_Real(ctx)
	--[[
	local r = 0;
	math.randomseed(ctx.seed);
	for i = 1, ctx.sequence + 1 do
		r = math.random();
	end

	ctx.sequence = ctx.sequence + 1;

	return r;
	]]

	return math.random();
end

function ll.Random_Int32(ctx)
	--[[
	local r = 0;
	math.randomseed(ctx.seed);
	for i = 1, ctx.sequence + 1 do
		r = math.random(-2147483648, 2147483647);
	end

	ctx.sequence = ctx.sequence + 1;

	return r;
	]]

	return math.random(-2147483648, 2147483647);
end

function ll.Random_UInt32(ctx)
	--[[
	local r = 0;
	math.randomseed(ctx.seed);
	for i = 1, ctx.sequence + 1 do
		r = math.random(0, 4294967295);
	end

	ctx.sequence = ctx.sequence + 1;

	return r;
	]]

	return math.random(0, 4294967295);
end

function ll.Random_Range(ctx, floor, ceiling)
	--[[
	local r = 0;
	math.randomseed(ctx.seed);
	for i = 1, ctx.sequence + 1 do
		r = math.random(floor, ceiling);
	end

	ctx.sequence = ctx.sequence + 1;

	return r;
	]]
	return math.random(floor, ceiling);
end

-----------------------------------------------------------------------------
-- tree
-----------------------------------------------------------------------------

function ll.Tree_AddChild(tree, child)
	child.root = tree;

	if tree.child == nil then
		tree.child = child;
	else
		if tree.child.sibling == nil then
			tree.child.sibling = child;
		else
			local tail = tree.child;
			while tail.sibling ~= nil do
				tail = tail.sibling;
			end
			tail.sibling = child;
		end
	end
end

function ll.Tree_AddSibling(tree, sibling)
	sibling.root = tree.root;
	local tail = tree.sibling;
	while tail.sibling ~= nil do
		tail = tail.sibling;
	end

	tail.sibling = sibling;
end

function ll.Tree_DumpToStack(tree)
	local stack = {};
	local tempstack = {};

	table.insert(tempstack, tree);

	while #tempstack ~= 0 do
		local tnode = table.remove(tempstack, 1);

		local temp = tnode.child;
		while temp ~= nil do
			table.insert(tempstack, 1, temp);
			temp = temp.sibling;
		end

		-- leaf
		if tnode.child == nil then
			table.insert(stack, 1, tnode);
		end
	end

	return stack;
end
