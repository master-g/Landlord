#include "lualandlord.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
    
/*
 * ************************************************************
 * Lua compatible macro
 * ************************************************************
 */
#if LUA_VERSION_NUM == 501
    #define luaL_len    luaL_getn
#else
    
#endif

#ifdef __cplusplus
}
#endif

#include "common.h"
#include "mt19937.h"
#include "medalgo.h"
#include "card.h"
#include "deck.h"
#include "hand.h"

/*
 * ************************************************************
 * string
 * ************************************************************
 */

#define LUALANDLORD                 "LuaLandlord"
#define LUALANDLORD_MT19937         "MT19937"
#define LUALANDLORD_MEDALGO         "MEDAlgo"
#define LUALANDLORD_CARD            "Card"
#define LUALANDLORD_HAND            "Hand"

#define LUALANDLORD_MT19937_META    LUALANDLORD "." LUALANDLORD_MT19937
#define LUALANDLORD_MEDALGO_META    LUALANDLORD "." LUALANDLORD_MEDALGO
#define LUALANDLORD_CARD_META       LUALANDLORD "." LUALANDLORD_CARD
#define LUALANDLORD_HAND_META       LUALANDLORD "." LUALANDLORD_HAND

/*
 * ************************************************************
 * C function
 * ************************************************************
 */

#define LL_CheckType(L, idx, meta)  luaL_checkudata(L, idx, meta)

static void LL_Type_New(lua_State *L, size_t size, const char *meta)
{
    lua_newuserdata(L, size);
    luaL_getmetatable(L, meta);
    lua_setmetatable(L, -2);
}

/*
 * ************************************************************
 * MT19937 random
 * ************************************************************
 */

static int LL_MT19937_New(lua_State *L)
{
    LL_Type_New(L, sizeof(mt19937_t), LUALANDLORD_MT19937_META);
    
    return 1;
}

static int LL_MT19937_Init(lua_State *L)
{
    mt19937_t *mt = (mt19937_t *)LL_CheckType(L, 1, LUALANDLORD_MT19937_META);
    
    if (mt != NULL)
    {
        uint32_t seed = (uint32_t)luaL_checkinteger(L, 2);
        Random_Init(mt, seed);
    }
    else
    {
        return luaL_error(L, "`mt19937' expected");
    }
    
    return 0;
}

static int LL_MT19937_InitWithArray(lua_State *L)
{
    mt19937_t *mt = (mt19937_t *)LL_CheckType(L, 1, LUALANDLORD_MT19937_META);
    
    if (mt != NULL)
    {
        int i, n;
        uint32_t *arr = NULL;
        
        luaL_checktype(L, 2, LUA_TTABLE);
        n = luaL_len(L, 1);
        
        if (n > MT_N || n <= 0)
            return luaL_error(L, "invalid array");
        
        arr = (uint32_t *)malloc(sizeof(uint32_t) * n);
        
        for (i = 1; i <= n; i++)
        {
            lua_rawgeti(L, 1, i);
            arr[i - 1] = (uint32_t)luaL_checkinteger(L, -1);
            lua_pop(L, 1);
        }
        
        Random_InitWithArray(mt, arr, n);
        
        free(arr);
    }
    else
    {
        return luaL_error(L, "`mt19937 expected");
    }
    
    return 0;
}

static int LL_MT19937_UInt32(lua_State *L)
{
    mt19937_t *mt = (mt19937_t *)LL_CheckType(L, 1, LUALANDLORD_MT19937_META);
    
    if (mt != NULL)
    {
        uint32_t val = Random_uint32(mt);
        lua_pushinteger(L, val);
    }
    else
    {
        return luaL_error(L, "`mt19937 expected'");
    }
    
    return 1;
}

static int LL_MT19937_Int32(lua_State *L)
{
    mt19937_t *mt = (mt19937_t *)LL_CheckType(L, 1, LUALANDLORD_MT19937_META);
    
    if (mt != NULL)
    {
        int32_t val = Random_Int32(mt);
        lua_pushinteger(L, val);
    }
    else
    {
        return luaL_error(L, "`mt19937 expected'");
    }
    
    return 1;
}

static int LL_MT19937_Real(lua_State *L)
{
    mt19937_t *mt = (mt19937_t *)LL_CheckType(L, 1, LUALANDLORD_MT19937_META);
    
    if (mt != NULL)
    {
        double val = Random_real_0_1(mt);
        lua_pushnumber(L, val);
    }
    else
    {
        return luaL_error(L, "`mt19937' expected");
    }
    
    return 1;
}

/*
 * ************************************************************
 * Card 
 * ************************************************************
 */
static int LL_Card_Make(lua_State *L)
{
    int suit, rank;
    
    suit = (int)luaL_checkinteger(L, 1);
    rank = (int)luaL_checkinteger(L, 2);
    
    lua_pushinteger(L, Card_Make(suit, rank));
    
    return 1;
}

static int LL_Card_Rank(lua_State *L)
{
    int card = (int)luaL_checkinteger(L, 1);
    lua_pushinteger(L, CARD_RANK(card));
    
    return 1;
}

static int LL_Card_Suit(lua_State *L)
{
    int card = (int)luaL_checkinteger(L, 1);
    lua_pushinteger(L, CARD_SUIT(card));
    
    return 1;
}

static int LL_Card_Red(lua_State *L)
{
    int card = (int)luaL_checkinteger(L, 1);
    lua_pushboolean(L, CARD_RED(card));
    
    return 1;
}

/*
 * ************************************************************
 * CardArray
 * ************************************************************
 */

static int LL_CardArray_New(lua_State *L)
{
    LL_Type_New(L, sizeof(mt19937_t), LUALANDLORD_CARD_META);
    
    return 1;
}

static int LL_CardArray_Peek(lua_State *L)
{
    int idx = 0;
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARD_META);
    
    idx = (int)luaL_checkinteger(L, 2);
    lua_pushinteger(L, arr->cards[idx]);
    
    return 1;
}

static int LL_CardArray_Clear(lua_State *L)
{
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARD_META);
    
    if (arr != NULL)
    {
        memset(arr, 0, sizeof(card_array_t));
    }
    else
    {
        luaL_error(L, "`cardarray' expected");
    }
    
    return 0;
}

static int LL_CardArray_Copy(lua_State *L)
{
    card_array_t *origin = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARD_META);
    if (origin != NULL)
    {
        card_array_t *copy = (card_array_t *)lua_newuserdata(L, sizeof(card_array_t));
        
        CardArray_Copy(copy, origin);
        
        luaL_getmetatable(L, LUALANDLORD_CARD_META);
        lua_setmetatable(L, -2);
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 1;
}

static int LL_CardArray_IsFull(lua_State *L)
{
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARD_META);
    
    if (arr != NULL)
    {
        lua_pushboolean(L, CardArray_IsFull(arr));
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 1;
}

static int LL_CardArray_IsEmpty(lua_State *L)
{
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARD_META);
    
    if (arr != NULL)
    {
        lua_pushboolean(L, CardArray_IsEmpty(arr));
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 1;
}

static int LL_CardArray_Capacity(lua_State *L)
{
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARD_META);
    
    if (arr != NULL)
    {
        lua_pushinteger(L, CardArray_Capacity(arr));
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 1;
}

static int LL_CardArray_InitFromString(lua_State *L)
{
    
    return 1;
}




int luaopen_landlord(lua_State *L)
{
    static const struct luaL_Reg ll_MT19937[] =
    {
        { "MT19937_New", LL_MT19937_New },
        { NULL, NULL }
    };
    
    static const struct luaL_Reg ll_MT19937_meta[] =
    {
        { "init",           LL_MT19937_Init },
        { "initWithArray",  LL_MT19937_InitWithArray },
        { "uint32",         LL_MT19937_UInt32 },
        { "int32",          LL_MT19937_Int32 },
        { NULL, NULL }
    };
    
    luaL_newmetatable(L, LUALANDLORD_MT19937_META);
    
#if LUA_VERSION_NUM == 501
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    
    luaL_openlib(L, NULL, ll_MT19937_meta, 0);
    
    luaL_openlib(L, LUALANDLORD, ll_MT19937, 0);
#else
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    
    luaL_setfuncs(L, LL_MT19937_meta, 0);
    
    luaL_newlib(L, LL_MT19937);
#endif
    
    return 0;
}

double Random_real_0_1(mt19937_t *context);

