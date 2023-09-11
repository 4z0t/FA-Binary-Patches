#pragma once

#include "global.h"

#define LUA_IDSIZE 60

struct lua_Debug
{
    int event;
    const char *name;           // (n)
    const char *namewhat;       // (n) 'global', 'local', 'field', 'method'
    const char *what;           // (S) 'Lua', 'C', 'main', 'tail'
    const char *source;         // (S)
    int currentline;            // (l)
    int nups;                   // (u) number of upvalues
    int linedefined;            // (S)
    char short_src[LUA_IDSIZE]; // (S)
    /* private part */
    int i_ci; // active function
};

class LuaState;
// lua.org/source/5.0/lstate.h.html#lua_State
typedef struct
{
    uint8_t pad[0x44];
    LuaState *LuaState;
} lua_State;

typedef struct luaL_Buffer luaL_Buffer;
typedef const char *(*lua_Chunkreader)(lua_State *L, void *data, size_t *size);
typedef void (*lua_Hook)(lua_State *L, lua_Debug *ar);
typedef void (*userGCFunction)(void *);
typedef int (*lua_CFunction)(lua_State *L);
typedef float lua_Number;

typedef struct luaL_reg
{
    const char *name;
    lua_CFunction func;
} luaL_reg;

#define LUA_MULTRET (-1)

#define LUA_REGISTRYINDEX (-10000)
#define LUA_GLOBALSINDEX (-10001)
#define lua_upvalueindex(i) (LUA_GLOBALSINDEX - (i))

#define LUA_OK 0
#define LUA_ERRRUN 1
#define LUA_ERRFILE 2
#define LUA_ERRSYNTAX 3
#define LUA_ERRMEM 4
#define LUA_ERRERR 5

#define LUA_TNONE (-1)
#define LUA_TNIL 0
#define LUA_TBOOLEAN 1
#define LUA_TLIGHTUSERDATA 2
#define LUA_TNUMBER 3
#define LUA_TSTRING 4
#define LUA_TTABLE 5
#define LUA_CFUNCTION 6
#define LUA_TFUNCTION 7
#define LUA_TUSERDATA 8
#define LUA_TTHREAD 9

#define LUA_HOOKCALL 0
#define LUA_HOOKRET 1
#define LUA_HOOKLINE 2
#define LUA_HOOKCOUNT 3
#define LUA_HOOKTAILRET 4

#define LUA_MASKCALL (1 << LUA_HOOKCALL)
#define LUA_MASKRET (1 << LUA_HOOKRET)
#define LUA_MASKLINE (1 << LUA_HOOKLINE)
#define LUA_MASKCOUNT (1 << LUA_HOOKCOUNT)

#define LUA_NOREF (-2)
#define LUA_REFNIL (-1)

#define lua_boxpointer(L, u) \
    (*(void **)(lua_newuserdata(L, sizeof(void *))) = (u))
#define lua_unboxpointer(L, i) (*(void **)(lua_touserdata(L, i)))

#define lua_pop(L, n) lua_settop(L, -(n)-1)

#define lua_register(L, n, f) \
    (lua_pushstring(L, n),    \
     lua_pushcfunction(L, f), \
     lua_settable(L, LUA_GLOBALSINDEX))

#define lua_pushcfunction(L, f) lua_pushcclosure(L, f, 0)

#define lua_iscfunction(L, n) (lua_type(L, n) == LUA_CFUNCTION)
#define lua_isfunction(L, n) (lua_type(L, n) == LUA_TFUNCTION)
#define lua_istable(L, n) (lua_type(L, n) == LUA_TTABLE)
#define lua_islightuserdata(L, n) (lua_type(L, n) == LUA_TLIGHTUSERDATA)
#define lua_isuserdata(L, n) (lua_type(L, n) == LUA_TUSERDATA)
#define lua_isnil(L, n) (lua_type(L, n) == LUA_TNIL)
#define lua_isboolean(L, n) (lua_type(L, n) == LUA_TBOOLEAN)
#define lua_isnone(L, n) (lua_type(L, n) == LUA_TNONE)
#define lua_isnoneornil(L, n) (lua_type(L, n) <= 0)

#define lua_pushliteral(L, s) \
    lua_pushlstring(L, "" s, (sizeof(s) / sizeof(char)) - 1)

#define lua_getregistry(L) lua_pushvalue(L, LUA_REGISTRYINDEX)
#define lua_setglobal(L, s) \
    (lua_pushstring(L, s), lua_insert(L, -2), lua_settable(L, LUA_GLOBALSINDEX))
#define lua_getglobal(L, s) \
    (lua_pushstring(L, s), lua_gettable(L, LUA_GLOBALSINDEX))

#define lua_ref(L, lock) ((lock) ? luaL_ref(L, LUA_REGISTRYINDEX) : (lua_pushstring(L, "unlocked references are obsolete"), lua_error(L), 0))
#define lua_unref(L, ref) luaL_unref(L, LUA_REGISTRYINDEX, (ref))
#define lua_getref(L, ref) lua_rawgeti(L, LUA_REGISTRYINDEX, ref)

typedef union
{
    void *p;
    float n;
    int b;
} Value;

// lua.org/source/5.0/lobject.h.html#TObject
typedef struct
{
    int tt;
    Value value;

    /* Types:
     -1 - None
      0 - Nil
      1 - Boolean
      2 - LightUserData
      3 - Number
      4 - String
      5 - Table
      6 - CFunction
      7 - Function
      8 - UserData
      9 - Thread
    */
} TObject;
VALIDATE_SIZE(TObject, 8);

typedef unsigned char lu_byte;
class Table
{
public:
    int pad0;
    int pad1;
    lu_byte tt;
    lu_byte lsizenode; /* log2 of size of `node' array */
    lu_byte marked;    /* 1<<p means tagmethod(p) is not present */
    lu_byte flags;
    struct Table *metatable;
    TObject *array; /* array part */
    int pad2;
    int pad3;
    int pad4;
    int sizearray; /* size of `array' array */
};
VALIDATE_SIZE(Table, 0x24);

// namespace gpg
class RRef
{
public:
    void *d;
    void *t;
};
class RType
{
};

// namespace LuaPlus
class LuaState;

class LuaStackObject
{
public:
    LuaState *m_state;
    int m_stackIndex;
};
VALIDATE_SIZE(LuaStackObject, 8);

class LuaObject
{ // 0x14 bytes
public:
    __thiscall LuaObject() ADDR(0x9072a0);
    __thiscall LuaObject(LuaState *state) ADDR(0x908970);
    __thiscall LuaObject(LuaState *state, int index) ADDR(0x9089c0);
    __thiscall LuaObject(const LuaObject *obj) ADDR(0x908a40);
    __thiscall LuaObject(const LuaStackObject *stack) ADDR(0x908a70);
    __thiscall LuaObject(LuaState *state, const TObject *obj) ADDR(0x9089f0);
    __thiscall ~LuaObject() ADDR(0x9075d0);

    LuaObject *operator=(const LuaObject *obj)
    {
        return ((__thiscall LuaObject * (*)(LuaObject *, const LuaObject *))0x908ab0)(this, obj);
    }
    LuaObject *operator=(const LuaStackObject *stack)
    {
        return ((__thiscall LuaObject * (*)(LuaObject *, const LuaStackObject *))0x908b00)(this, stack);
    }
    LuaObject operator[](int key)
    {
        LuaObject out;
        ((__thiscall void (*)(LuaObject *, LuaObject *, int))0x9091e0)(this, &out, key);
        return out;
    }
    LuaObject operator[](const char *key)
    {
        LuaObject out;
        ((__thiscall void (*)(LuaObject *, LuaObject *, const char *))0x908f60)(this, &out, key);
        return out;
    }
    bool __thiscall GetBoolean() ADDR(0x907c90);

    bool __thiscall IsBoolean() ADDR(0x9078d0);

    bool __thiscall IsConvertibleToString() ADDR(0x9077c0);

    bool __thiscall IsFunction() ADDR(0x907810);

    bool __thiscall IsInteger() ADDR(0x907350);

    bool __thiscall IsNil() ADDR(0x9072f0);

    bool __thiscall IsNumber() ADDR(0x907360);

    bool __thiscall IsString() ADDR(0x907370);

    bool __thiscall IsTable() ADDR(0x907310);

    bool __thiscall IsUserData() ADDR(0x907320);

    void __thiscall Clone(LuaObject *out) ADDR(0x90a180);

    void __thiscall CreateTable(LuaObject *out, const char *key, int narray, int lnhash) ADDR(0x908c10);
    void __thiscall CreateTable(LuaObject *out, int key, int narray, int lnhash) ADDR(0x908ca0);
    void __thiscall GetByIndex(LuaObject *out, int index) ADDR(0x908df0);
    void __thiscall GetByName(LuaObject *out, const char *name) ADDR(0x90a160);
    void __thiscall GetByObject(LuaObject *out, const LuaObject *obj) ADDR(0x908e70);
    void __thiscall GetMetaTable(LuaObject *out) ADDR(0x908ba0);
    void __thiscall Lookup(LuaObject *out, const char *key) ADDR(0x9093b0);
    void __thiscall PushStack(LuaStackObject *out, LuaState *state) ADDR(0x907d80);
    void __thiscall PushStack(lua_State *L) ADDR(0x907d10);
    LuaState *__thiscall GetActiveState() ADDR(0x9072b0);
    const char *__thiscall GetString() ADDR(0x907a90);
    const char *__thiscall ToString() ADDR(0x9073e0);
    const char *__thiscall TypeName() ADDR(0x908b50);
    lua_Number __thiscall GetNumber() ADDR(0x907970);
    lua_Number __thiscall GetDouble() ADDR(0x907a30);
    lua_Number __thiscall ToNumber() ADDR(0x9073b0);
    void __thiscall AssignNewUserData(RRef *out, LuaState *state, const RRef *rRef) ADDR(0x909840);
    void __thiscall AssignNewUserData(RRef *out, LuaState *state, const RType *rType) ADDR(0x9097d0);
    void __thiscall GetUserData(RRef *out) ADDR(0x907bc0);
    int __thiscall GetCount() ADDR(0x907f50);
    int __thiscall GetInteger() ADDR(0x907910);
    int __thiscall GetN() ADDR(0x907e50);
    int __thiscall GetTableCount() ADDR(0x90a410);
    int __thiscall IsPassed() ADDR(0x907440);

    int __thiscall Type() ADDR(0x9076d0);
    lua_State *__thiscall GetActiveCState() ADDR(0x9072c0);

    void __thiscall AssignBoolean(LuaState *state, bool value) ADDR(0x909600);
    void __thiscall AssignInteger(LuaState *state, int value) ADDR(0x909650);
    void __thiscall AssignNewTable(LuaState *state, int narray, int lnhash) ADDR(0x909940);
    void __thiscall AssignNil(LuaState *state) ADDR(0x9095c0);
    void __thiscall AssignNumber(LuaState *state, float value) ADDR(0x9096a0);
    void __thiscall AssignString(LuaState *state, const char *value) ADDR(0x909750);
    void __thiscall AssignTObject(LuaState *state, const TObject *value) ADDR(0x9099b0);
    void __thiscall AssignThread(LuaState *state) ADDR(0x9096f0);
    void __thiscall Insert(LuaObject *value) ADDR(0x909af0);
    void __thiscall Insert(int key, LuaObject *value) ADDR(0x909ce0);
    void __thiscall Register(const char *name, lua_CFunction func, int nupvalues) ADDR(0x907630);
    void __thiscall Reset() ADDR(0x9075f0);
    void __thiscall SetBoolean(const char *key, bool value) ADDR(0x9080c0);
    void __thiscall SetInteger(const char *key, int value) ADDR(0x9081f0);
    void __thiscall SetInteger(int key, int value) ADDR(0x908240);
    void __thiscall SetMetaTable(const LuaObject *value) ADDR(0x907e00);
    void __thiscall SetN(int n) ADDR(0x907ed0);
    void __thiscall SetNil(const char *key) ADDR(0x907fa0);
    void __thiscall SetNil(int key) ADDR(0x907ff0);
    void __thiscall SetNumber(const char *key, float value) ADDR(0x908320);
    void __thiscall SetNumber(int key, float value) ADDR(0x908370);
    void __thiscall SetObject(const LuaObject *key, const LuaObject *value) ADDR(0x908810);
    void __thiscall SetObject(const char *key, const LuaObject *value) ADDR(0x908760);
    void __thiscall SetObject(int key, const LuaObject *value) ADDR(0x9087a0);
    void __thiscall SetString(const char *key, const char *value) ADDR(0x908450);
    void __thiscall SetString(int key, const char *value) ADDR(0x9084e0);
    void __thiscall TypeError(const char *msg) ADDR(0x9072d0);

    // private
    void __thiscall AddToUsedList(LuaState *state) ADDR(0x908890);
    void __thiscall AddToUsedList(LuaState *state, const TObject *obj) ADDR(0x9088e0);
    void __thiscall SetTableHelper(const char *key, const TObject *value) ADDR(0x9074b0);

    LuaObject *m_next;
    LuaObject *m_prev;
    LuaState *m_state;
    TObject m_object;
};
VALIDATE_SIZE(LuaObject, 0x14);

class LuaState
{ // 0x34 bytes
public:
    enum StandardLibraries
    {
        LIB_NONE,
        LIB_BASE,
        LIB_OSIO
    };

    __thiscall LuaState(enum StandardLibraries libs) ADDR(0x90ac10);
    __thiscall LuaState(LuaState *parentState) ADDR(0x90a520);
    __thiscall LuaState(int Unused) ADDR(0x90a5d0);
    __thiscall ~LuaState() ADDR(0x90a600);
    void __thiscall GetGlobal(LuaObject *out, const char *key) ADDR(0x4579d0);
    void __thiscall GetGlobals(LuaObject *out) ADDR(0x90a690);
    LuaState *__thiscall GetActiveState() ADDR(0x90bee0);

    const char *__thiscall CheckString(int narg) ADDR(0x912d10);

    int __thiscall ArgError(int narg, const char *msg) ADDR(0x90bf70);

    int __cdecl Error(const char *fmt, ...) ADDR(0x90c1d0);

    lua_State *__thiscall GetActiveCState() ADDR(0x90bef0);

    void __thiscall CheckAny(int narg) ADDR(0x923f20);

    // private
    void __thiscall Init(enum StandardLibraries libs) ADDR(0x90aad0);

    lua_State *m_state;
    void *ForMultipleThreads;
    bool m_ownState;
    LuaObject m_threadObj;
    LuaState *m_rootState;
    struct MiniLuaObject
    {
        LuaObject *m_next; // only valid when in free list
        LuaObject *m_prev; // only valid when in used list
    } m_headObject, m_tailObject;
};
VALIDATE_SIZE(LuaState, 0x34);

bool __cdecl LuaPlusH_next(LuaState *, const LuaObject *, LuaObject *, LuaObject *) ADDR(0x90a6b0);
bool __cdecl lua_toboolean(lua_State *l, int index) ADDR(0x90ca40);
TObject *__cdecl negindex(lua_State *, int) ADDR(0x90c340);
char *__cdecl luaL_prepbuffer(luaL_Buffer *) ADDR(0x90e260);
const char *__cdecl luaL_checklstring(lua_State *, int, unsigned int *) ADDR(0x90eaa0);
const char *__cdecl luaL_optlstring(lua_State *, int, const char *, unsigned int *) ADDR(0x90eb10);
const char *__cdecl lua_getlocal(lua_State *, const lua_Debug *, int) ADDR(0x912680);
const char *__cdecl lua_getupvalue(lua_State *, int, int) ADDR(0x90d9a0);
const char *__cdecl lua_pushfstring(lua_State *, const char *, ...) ADDR(0x90ce90);
const char *__cdecl lua_pushvfstring(lua_State *, const char *, char *) ADDR(0x90ce50);
const char *__cdecl lua_setlocal(lua_State *, const lua_Debug *, int) ADDR(0x9126f0);
const char *__cdecl lua_setupvalue(lua_State *, int, int) ADDR(0x90da00);
const char *__cdecl lua_tostring(lua_State *, int) ADDR(0x90ca90);
const char *__cdecl lua_typename(lua_State *, int) ADDR(0x90c780);
float __cdecl luaL_checknumber(lua_State *, int) ADDR(0x90eb70);
float __cdecl luaL_optnumber(lua_State *, int, float) ADDR(0x90ebf0);
float __cdecl lua_tonumber(lua_State *, int) ADDR(0x90c9f0);
void __cdecl lua_newuserdata(RRef *ret, lua_State *, const RType *) ADDR(0x90d7e0);
RRef *__cdecl lua_touserdata(RRef *, lua_State *, int) ADDR(0x90cbb0);
inline RRef lua_touserdata(lua_State *l, int index)
{
    RRef ref;
    lua_touserdata(&ref, l, index);
    return ref;
}
int __cdecl luaL_argerror(lua_State *, int, const char *) ADDR(0x90e900);
int __cdecl luaL_callmeta(lua_State *, int, const char *) ADDR(0x90dda0);
int __cdecl luaL_error(lua_State *, const char *, ...) ADDR(0x90dbf0);
FDecl(0x90dc20, luaL_findstring, int __cdecl (*)(const char *, const char *const list[]));
int __cdecl luaL_getmetafield(lua_State *, int, const char *) ADDR(0x90dd40);
int __cdecl luaL_getn(lua_State *, int) ADDR(0x90e090);
int __cdecl luaL_loadbuffer(lua_State *, const char *, unsigned int, const char *) ADDR(0x90e760);
int __cdecl luaL_loadfile(lua_State *, const char *) ADDR(0x90e5d0);
int __cdecl luaL_newmetatable(lua_State *, const char *) ADDR(0x90dc70);
int __cdecl luaL_typerror(lua_State *, int, const char *) ADDR(0x90e9a0);
int __cdecl lua_checkstack(lua_State *, int) ADDR(0x90c460);
int __cdecl lua_dobuffer(lua_State *, const char *, unsigned int, const char *) ADDR(0x90e870);
int __cdecl lua_dostring(lua_State *, const char *) ADDR(0x90e8d0);
int __cdecl lua_error(lua_State *) ADDR(0x90d6c0);
int __cdecl lua_getgccount(lua_State *) ADDR(0x90d660);
int __cdecl lua_getgcthreshold(lua_State *) ADDR(0x90d650);
int __cdecl lua_gethookcount(lua_State *) ADDR(0x9125d0);
int __cdecl lua_gethookmask(lua_State *) ADDR(0x9125c0);
int __cdecl lua_getinfo(lua_State *, const char *, lua_Debug *) ADDR(0x9132f0);
int __cdecl lua_getmetatable(lua_State *, int) ADDR(0x90d180);
int __cdecl lua_getn(lua_State *, int) ADDR(0x90ad30);
int __cdecl lua_getstack(lua_State *, int, lua_Debug *) ADDR(0x9125e0);
int __cdecl lua_gettop(lua_State *) ADDR(0x90c590);
int __cdecl lua_isnumber(lua_State *, int) ADDR(0x90c7a0);
int __cdecl lua_isstring(lua_State *, int) ADDR(0x90c800);
int __cdecl lua_lessthan(lua_State *, int, int) ADDR(0x90c980);
int __cdecl lua_load(lua_State *, lua_Chunkreader, void *, const char *) ADDR(0x90d5c0);
int __cdecl lua_next(lua_State *, int) ADDR(0x90d6d0);
int __cdecl lua_pcall(lua_State *, int, int) ADDR(0x90d430);
int __cdecl lua_rawequal(lua_State *, int, int) ADDR(0x90c890);
int __cdecl lua_resume(lua_State *, int) ADDR(0x914610);
int __cdecl lua_setfenv(lua_State *, int) ADDR(0x90d3b0);
int __cdecl lua_sethook(lua_State *, lua_Hook, int, int) ADDR(0x912560);
int __cdecl lua_setmetatable(lua_State *, int) ADDR(0x90d340);
int __cdecl lua_traceback(lua_State *, const char *, int) ADDR(0x911ea0);
int __cdecl lua_type(lua_State *, int) ADDR(0x90c740);
int __cdecl lua_yield(lua_State *, int) ADDR(0x913e40);
int __cdecl luaopen_base(lua_State *) ADDR(0x90fd90);
int __cdecl luaopen_debug(lua_State *) ADDR(0x9124c0);
int __cdecl luaopen_loadlib(lua_State *) ADDR(0x91a4b0);
int __cdecl luaopen_math(lua_State *) ADDR(0x91a110);
int __cdecl luaopen_serialize(lua_State *) ADDR(0x923690);
int __cdecl luaopen_string(lua_State *) ADDR(0x926ef0);
lua_State *__cdecl lua_newthread(lua_State *) ADDR(0x90c530);
lua_State *__cdecl lua_open() ADDR(0x9246d0);
lua_State *__cdecl lua_tothread(lua_State *, int) ADDR(0x90cc50);
unsigned int __cdecl lua_strlen(lua_State *, int) ADDR(0x90cb10);
lua_Hook __cdecl lua_gethook(lua_State *) ADDR(0x9125b0);
void __cdecl luaL_addlstring(luaL_Buffer *, const char *, unsigned int) ADDR(0x90e2a0);
void __cdecl luaL_addstring(luaL_Buffer *, const char *) ADDR(0x90e300);
void __cdecl luaL_addvalue(luaL_Buffer *) ADDR(0x90e370);
void __cdecl luaL_buffinit(lua_State *, luaL_Buffer *) ADDR(0x90e400);
void __cdecl luaL_checkany(lua_State *, int) ADDR(0x90ea70);
void __cdecl luaL_checkstack(lua_State *, int, const char *) ADDR(0x90dd10);
void __cdecl luaL_checktype(lua_State *, int, int) ADDR(0x90ea20);
void __cdecl luaL_getmetatable(lua_State *, const char *) ADDR(0x90dcf0);
void __cdecl luaL_openlib(lua_State *, const char *, const luaL_reg *, int) ADDR(0x90de00);
void __cdecl luaL_pushresult(luaL_Buffer *) ADDR(0x90e330);
void __cdecl luaL_setn(lua_State *, int, int) ADDR(0x90dfb0);
void __cdecl luaL_where(lua_State *, int) ADDR(0x90db80);
void __cdecl lua_call(lua_State *, int, int) ADDR(0x90d400);
void __cdecl lua_close(lua_State *) ADDR(0x9243e0);
void __cdecl lua_concat(lua_State *, int) ADDR(0x90d740);
void __cdecl lua_getfenv(lua_State *, int) ADDR(0x90d1f0);
void __cdecl lua_gettable(lua_State *, int) ADDR(0x90d000);
void __cdecl lua_insert(lua_State *, int) ADDR(0x90c640);
void __cdecl lua_newtable(lua_State *) ADDR(0x90d110);
void __cdecl lua_pushboolean(lua_State *, int) ADDR(0x90cf80);
void __cdecl lua_pushcclosure(lua_State *, lua_CFunction, int) ADDR(0x90ced0);
void __cdecl lua_pushlightuserdata(lua_State *, void *) ADDR(0x90cfc0);
void __cdecl lua_pushlstring(lua_State *, const char *, unsigned int) ADDR(0x90cd80);
void __cdecl lua_pushnil(lua_State *) ADDR(0x90cd00);
void __cdecl lua_pushnumber(lua_State *, float) ADDR(0x90cd40);
void __cdecl lua_pushstring(lua_State *, const char *) ADDR(0x90cdf0);
void __cdecl lua_pushvalue(lua_State *, int) ADDR(0x90c6e0);
void __cdecl lua_rawget(lua_State *, int) ADDR(0x90d050);
void __cdecl lua_rawgeti(lua_State *, int, int) ADDR(0x90d0a0);
void __cdecl lua_rawset(lua_State *, int) ADDR(0x90d2a0);
void __cdecl lua_rawseti(lua_State *, int, int) ADDR(0x90d2f0);
void __cdecl lua_remove(lua_State *, int) ADDR(0x90c5f0);
void __cdecl lua_replace(lua_State *, int) ADDR(0x90c690);
void __cdecl lua_setdefaultmetatable(lua_State *, int) ADDR(0x90ad00);
void __cdecl lua_setgcthreshold(lua_State *, int) ADDR(0x90d670);
void __cdecl lua_setglobaluserdata(lua_State *, void *) ADDR(0x924060);
void __cdecl lua_setstateuserdata(lua_State *, void *) ADDR(0x9240b0);
void __cdecl lua_settable(lua_State *, int) ADDR(0x90d260);
void __cdecl lua_settop(lua_State *, int) ADDR(0x90c5a0);
void __cdecl lua_setusergcfunction(lua_State *, userGCFunction) ADDR(0x924080);
void __cdecl lua_xmove(lua_State *, lua_State *, int) ADDR(0x90c4c0);
void const *__cdecl lua_topointer(lua_State *, int) ADDR(0x90cc90);
void *__cdecl lua_getglobaluserdata(lua_State *) ADDR(0x924050);
void *__cdecl lua_getstateuserdata(lua_State *) ADDR(0x9240a0);
void *__cdecl lua_tolightuserdata(lua_State *, int) ADDR(0x90cc10);