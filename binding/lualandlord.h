/*
 *  lualandlord.h
 *  Landlord
 *
 *  Created by Master.G on 14-8-8.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#ifndef LUALANDLORD_H_
#define LUALANDLORD_H_

#ifdef __cplusplus
extern "C" {
#endif
	
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

int lua_landlord_open(lua_State *L);

#ifdef __cplusplus
}
#endif

#endif