#include "lualandlord.h"

#include "common.h"
#include "mt19937.h"
#include "medalgo.h"
#include "card.h"
#include "deck.h"
#include "hand.h"

/*============================================================*\
 * names
\*============================================================*/

#define LUALANDLORD                 "ll"
#define LUALANDLORD_MT19937         "mt19937"
#define LUALANDLORD_MEDALGO         "algo"
#define LUALANDLORD_CARD            "card"
#define LUALANDLORD_CARDARRAY       "cardarray"
#define LUALANDLORD_DECK            "deck"
#define LUALANDLORD_HAND            "hand"

#define LUALANDLORD_MT19937_META    LUALANDLORD "." LUALANDLORD_MT19937
#define LUALANDLORD_MEDALGO_META    LUALANDLORD "." LUALANDLORD_MEDALGO
#define LUALANDLORD_CARD_META       LUALANDLORD "." LUALANDLORD_CARD
#define LUALANDLORD_CARDARRAY_META  LUALANDLORD "." LUALANDLORD_CARDARRAY
#define LUALANDLORD_DECK_META       LUALANDLORD "." LUALANDLORD_DECK
#define LUALANDLORD_HAND_META       LUALANDLORD "." LUALANDLORD_HAND

/*============================================================*\
 * utils
\*============================================================*/

#define LL_CheckType(L, idx, meta)  luaL_checkudata(L, idx, meta)

static void* LL_Type_New(lua_State *L, size_t size, const char *meta)
{
    void *ud = lua_newuserdata(L, size);
    luaL_getmetatable(L, meta);
    lua_setmetatable(L, -2);
    
    return ud;
}

/*============================================================*\
 * MT19937
\*============================================================*/

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
        n = luaL_getn(L, 1);
        
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

static int LL_MT19937_open(lua_State *L)
{
    static const struct luaL_Reg ll_MT19937[] =
    {
        { "new", LL_MT19937_New },
        { NULL, NULL }
    };
    
    static const struct luaL_Reg ll_MT19937_meta[] =
    {
        { "init",           LL_MT19937_Init },
        { "initWithArray",  LL_MT19937_InitWithArray },
        { "uint32",         LL_MT19937_UInt32 },
        { "int32",          LL_MT19937_Int32 },
        { "real",           LL_MT19937_Real },
        { NULL, NULL }
    };
    
    luaL_newmetatable(L, LUALANDLORD_MT19937_META);
    
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    
    luaL_register(L, NULL, ll_MT19937_meta);
    luaL_register(L, LUALANDLORD_MT19937_META, ll_MT19937);

    return 1;
}

/*============================================================*\
 * Card
\*============================================================*/

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

static int LL_Card_open(lua_State *L)
{
    static const struct luaL_Reg ll_Card[] =
    {
        { "make", LL_Card_Make },
        { "rank", LL_Card_Rank },
        { "suit", LL_Card_Suit },
        { "isred", LL_Card_Red },
        { NULL, NULL }
    };
    
    luaL_register(L, LUALANDLORD_CARD_META, ll_Card);
    
    return 1;
}

/*============================================================*\
 * CardArray
\*============================================================*/

static int LL_CardArray_New(lua_State *L)
{
    LL_Type_New(L, sizeof(mt19937_t), LUALANDLORD_CARDARRAY_META);
    
    return 1;
}

static int LL_CardArray_Peek(lua_State *L)
{
    int idx = 0;
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    
    idx = (int)luaL_checkinteger(L, 2);
    lua_pushinteger(L, arr->cards[idx]);
    
    return 1;
}

static int LL_CardArray_Clear(lua_State *L)
{
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    
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
    card_array_t *origin = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    if (origin != NULL)
    {
        card_array_t *copy = (card_array_t *)lua_newuserdata(L, sizeof(card_array_t));
        
        CardArray_Copy(copy, origin);
        
        luaL_getmetatable(L, LUALANDLORD_CARDARRAY_META);
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
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    
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
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    
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
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    
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
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    
    if (arr != NULL)
    {
        const char *str = luaL_checkstring(L, 2);
        CardArray_InitFromString(arr, str);
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 0;
}

static int LL_CardArray_Reset(lua_State *L)
{
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    
    if (arr != NULL)
    {
        CardArray_Reset(arr);
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 0;
}

static int LL_CardArray_Concate(lua_State *L)
{
    card_array_t *arr1 = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    card_array_t *arr2 = (card_array_t *)LL_CheckType(L, 2, LUALANDLORD_CARDARRAY_META);
    
    if (arr1 != NULL && arr2 != NULL)
    {
        CardArray_Concate(arr1, arr2);
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 0;
}

static int LL_CardArray_Subtract(lua_State *L)
{
    card_array_t *arr1 = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    card_array_t *arr2 = (card_array_t *)LL_CheckType(L, 2, LUALANDLORD_CARDARRAY_META);
    
    if (arr1 != NULL && arr2 != NULL)
    {
        CardArray_Subtract(arr1, arr2);
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 0;
}

static int LL_CardArray_IsIdentity(lua_State *L)
{
    card_array_t *arr1 = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    card_array_t *arr2 = (card_array_t *)LL_CheckType(L, 2, LUALANDLORD_CARDARRAY_META);
    
    if (arr1 != NULL && arr2 != NULL)
    {
        lua_pushboolean(L, CardArray_IsIdentity(arr1, arr2));
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 1;
}

static int LL_CardArray_IsContain(lua_State *L)
{
    card_array_t *arr1 = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    card_array_t *arr2 = (card_array_t *)LL_CheckType(L, 2, LUALANDLORD_CARDARRAY_META);
    
    if (arr1 != NULL && arr2 != NULL)
    {
        lua_pushboolean(L, CardArray_IsContain(arr1, arr2));
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 1;
}

static int LL_CardArray_PushBack(lua_State *L)
{
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    int card = (int)luaL_checkinteger(L, 2);
    
    if (arr != NULL)
    {
        CardArray_PushBack(arr, card);
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 0;
}

static int LL_CardArray_PushFront(lua_State *L)
{
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    int card = (int)luaL_checkinteger(L, 2);
    
    if (arr != NULL)
    {
        CardArray_PushFront(arr, card);
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 0;
}

static int LL_CardArray_PopFront(lua_State *L)
{
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    
    if (arr != NULL)
    {
        int card = CardArray_PopFront(arr);
        lua_pushinteger(L, card);
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 1;
}

static int LL_CardArray_PopBack(lua_State *L)
{
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    
    if (arr != NULL)
    {
        int card = CardArray_PopBack(arr);
        lua_pushinteger(L, card);
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 1;
}

static int LL_CardArray_DropFront(lua_State *L)
{
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    
    if (arr != NULL)
    {
        int count = (int)luaL_checkinteger(L, 2);
        CardArray_DropFront(arr, count);
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 0;
}

static int LL_CardArray_DropBack(lua_State *L)
{
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    
    if (arr != NULL)
    {
        int count = (int)luaL_checkinteger(L, 2);
        CardArray_DropBack(arr, count);
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 0;
}

static int LL_CardArray_Insert(lua_State *L)
{
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    int before = (int)luaL_checkinteger(L, 2);
    int card = (int)luaL_checkinteger(L, 3);
    
    if (arr != NULL)
    {
        CardArray_Insert(arr, before, card);
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 0;
}

static int LL_CardArray_Remove(lua_State *L)
{
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    int where = (int)luaL_checkinteger(L, 2);
    
    if (arr != NULL)
    {
        int card = CardArray_Remove(arr, where);
        lua_pushinteger(L, card);
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 1;
}

static int LL_CardArray_RemoveCard(lua_State *L)
{
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    int card = (int)luaL_checkinteger(L, 2);
    
    if (arr != NULL)
    {
        card = CardArray_RemoveCard(arr, card);
        lua_pushinteger(L, card);
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 1;
}

static int LL_CardArray_PushBackCards(lua_State *L)
{
    card_array_t *arr1 = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    card_array_t *arr2 = (card_array_t *)LL_CheckType(L, 2, LUALANDLORD_CARDARRAY_META);
    int where = (int)luaL_checkinteger(L, 3);
    int count = (int)luaL_checkinteger(L, 4);
    
    if (arr1 != NULL && arr2 != NULL)
    {
        CardArray_PushBackCards(arr1, arr2, where, count);
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 0;
}

static int LL_CardArray_CopyRank(lua_State *L)
{
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    int rank = (int)luaL_checkinteger(L, 2);
    
    if (arr != NULL)
    {
        card_array_t *clone = (card_array_t *)LL_Type_New(L, sizeof(card_array_t), LUALANDLORD_CARDARRAY_META);
        CardArray_Clear(clone);
        CardArray_CopyRank(clone, arr, rank);
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 1;
}

static int LL_CardArray_RemoveRank(lua_State *L)
{
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    int rank = (int)luaL_checkinteger(L, 2);
    
    if (arr != NULL)
    {
        CardArray_RemoveRank(arr, rank);
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 0;
}

static int LL_CardArray_Sort(lua_State *L)
{
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    
    if (arr != NULL)
    {
        CardArray_Sort(arr, NULL);
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 0;
}

static int LL_CardArray_Reverse(lua_State *L)
{
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 1, LUALANDLORD_CARDARRAY_META);
    
    if (arr != NULL)
    {
        CardArray_Reverse(arr);
    }
    else
    {
        return luaL_error(L, "`cardarray' error");
    }
    
    return 0;
}

static int LL_CardArray_open(lua_State *L)
{
    static const struct luaL_Reg ll_CardArray[] =
    {
        { "new", LL_CardArray_New },
        { NULL, NULL }
    };
    
    static const struct luaL_Reg ll_CardArray_meta[] =
    {
        { "peek",           LL_CardArray_Peek           },
        { "clear",          LL_CardArray_Clear          },
        { "copy",           LL_CardArray_Copy           },
        { "isfull",         LL_CardArray_IsFull         },
        { "isempty",        LL_CardArray_IsEmpty        },
        { "capacity",       LL_CardArray_Capacity       },
        { "initfromstring", LL_CardArray_InitFromString },
        { "reset",          LL_CardArray_Reset          },
        { "concate",        LL_CardArray_Concate        },
        { "subtract",       LL_CardArray_Subtract       },
        { "isidentity",     LL_CardArray_IsIdentity     },
        { "iscontain",      LL_CardArray_IsContain      },
        { "pushback",       LL_CardArray_PushBack       },
        { "pushfront",      LL_CardArray_PushFront      },
        { "popfront",       LL_CardArray_PopFront       },
        { "popback",        LL_CardArray_PopBack        },
        { "dropfront",      LL_CardArray_DropFront      },
        { "dropback",       LL_CardArray_DropBack       },
        { "insert",         LL_CardArray_Insert         },
        { "remove",         LL_CardArray_Remove         },
        { "removecard",     LL_CardArray_RemoveCard     },
        { "pushbackcards",  LL_CardArray_PushBackCards  },
        { "copyrank",       LL_CardArray_CopyRank       },
        { "removerank",     LL_CardArray_RemoveRank     },
        { "sort",           LL_CardArray_Sort           },
        { "reverse",        LL_CardArray_Reverse        },
        { NULL, NULL }
    };
    
    luaL_newmetatable(L, LUALANDLORD_CARDARRAY_META);
    
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    
    luaL_register(L, NULL, ll_CardArray_meta);
    luaL_register(L, LUALANDLORD_CARDARRAY_META, ll_CardArray);
    
    return 1;
}

/*============================================================*\
 * Deck
\*============================================================*/

static int LL_Deck_New(lua_State *L)
{
    LL_Type_New(L, sizeof(mt19937_t), LUALANDLORD_DECK_META);
    
    return 1;
}

static int LL_Deck_Shuffle(lua_State *L)
{
    deck_t *deck = (deck_t *)LL_CheckType(L, 1, LUALANDLORD_DECK_META);
    mt19937_t *mt = (mt19937_t *)LL_CheckType(L, 2, LUALANDLORD_MT19937_META);
    
    if (deck != NULL)
    {
        Deck_Shuffle(deck, mt);
    }
    else
    {
        return luaL_error(L, "`deck' error");
    }
    
    return 0;
}

static int LL_Deck_Reset(lua_State *L)
{
    deck_t *deck = (deck_t *)LL_CheckType(L, 1, LUALANDLORD_DECK_META);
    
    if (deck != NULL)
    {
        Deck_Reset(deck);
    }
    else
    {
        return luaL_error(L, "`deck' error");
    }
    
    return 0;
}

static int LL_Deck_DealSingle(lua_State *L)
{
    deck_t *deck = (deck_t *)LL_CheckType(L, 1, LUALANDLORD_DECK_META);
    
    if (deck != NULL)
    {
        int card = Deck_DealSingle(deck);
        lua_pushinteger(L, card);
    }
    else
    {
        return luaL_error(L, "`deck' error");
    }
    
    return 1;
}

static int LL_Deck_RecycleSingle(lua_State *L)
{
    deck_t *deck = (deck_t *)LL_CheckType(L, 1, LUALANDLORD_DECK_META);
    int card = (int)luaL_checkinteger(L, 2);
    
    if (deck != NULL)
    {
        Deck_RecycleSingle(deck, card);
    }
    else
    {
        return luaL_error(L, "`deck' error");
    }
    
    return 0;
}

static int LL_Deck_Deal(lua_State *L)
{
    deck_t *deck = (deck_t *)LL_CheckType(L, 1, LUALANDLORD_DECK_META);
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 2, LUALANDLORD_CARDARRAY_META);
    int count = 0;
    int ret = 0;
    
    if (deck != NULL)
    {
        if (arr != NULL)
        {
            count = (int)luaL_checkinteger(L, 3);
            ret = 0;
        }
        else
        {
            count = (int)luaL_checkinteger(L, 2);
            arr = (card_array_t *)LL_Type_New(L, sizeof(card_array_t), LUALANDLORD_CARDARRAY_META);
            ret = 1;
        }
        
        Deck_Deal(deck, arr, count);
    }
    else
    {
        return luaL_error(L, "`deck' error");
    }
    
    return ret;
}

static int LL_Deck_Recycle(lua_State *L)
{
    deck_t *deck = (deck_t *)LL_CheckType(L, 1, LUALANDLORD_DECK_META);
    card_array_t *arr = (card_array_t *)LL_CheckType(L, 2, LUALANDLORD_CARDARRAY_META);
    
    if (deck != NULL && arr != NULL)
    {
        Deck_Recycle(deck, arr);
    }
    else
    {
        return luaL_error(L, "`deck' error");
    }
    
    return 0;
}

static int LL_Deck_open(lua_State *L)
{
    static const struct luaL_Reg ll_Deck[] =
    {
        { "new", LL_Deck_New },
        { NULL, NULL }
    };
    
    static const struct luaL_Reg ll_Deck_meta[] =
    {
        { "shuffle",        LL_Deck_Shuffle         },
        { "reset",          LL_Deck_Reset           },
        { "dealsingle",     LL_Deck_DealSingle      },
        { "recyclesingle",  LL_Deck_RecycleSingle   },
        { "deal",           LL_Deck_Deal            },
        { "recycle",        LL_Deck_Recycle         },
        { NULL, NULL }
    };
    
    luaL_newmetatable(L, LUALANDLORD_DECK_META);
    
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    
    luaL_register(L, NULL, ll_Deck_meta);
    luaL_register(L, LUALANDLORD_DECK_META, ll_Deck);
    
    return 1;
}


int luaopen_landlord(lua_State *L)
{
    LL_MT19937_open(L);
    LL_Card_open(L);
    LL_CardArray_open(L);
    LL_Deck_open(L);
    
    return 0;
}
