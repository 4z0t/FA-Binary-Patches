
#include "include/moho.h"
#include "include/CObject.hpp"

void __lua_createtable(/*lua_State *l, int narr, int nhash*/)
{
    asm( // copied from lua_newtable
        "push    esi;"
        "mov     esi, [esp+0x8];" // lua_State
        "mov     eax, [esi+0x10];"
        "mov     ecx, [eax+0x2C];"
        "cmp     ecx, [eax+0x24];"
        "mov  edx, [esp+0x0C];" // push narr
        "mov  ecx, [esp+0x10];" // push nhash
        "push    edi;"
        "jb      short loc_90D130;"
        "cmp     dword ptr [eax+0x28], 0;"
        "jnz     short loc_90D130;"
        "push    esi;"
        "call    0x915D90;"
        "add     esp, 4;"
        "loc_90D130: ;"
        "mov     edi, [esi+8];"
        "push    ecx;" // nhash
        "push    edx;" // narr
        "push    esi;"
        "call    0x00927320;" // luaH_new
        "movzx   edx, byte ptr [eax+4];"
        "mov     [edi], edx;"
        "mov     [edi+4], eax;"
        "mov     eax, [esi+8];"
        "mov     ecx, [esi+0x14];"
        "add     esp, 0x0C;"
        "cmp     eax, [ecx+4];"
        "jb      short loc_90D173;"
        "mov     edx, [esi+0x18];"
        "sub     edx, eax;"
        "mov     edi, 8;"
        "cmp     edx, edi;"
        "jg      short loc_90D16D;"
        "push    1;"
        "push    esi;"
        "call    0x913990;"
        "add     esp, 8;"
        "loc_90D16D:;"
        "add     [esi+8], edi;"
        "pop     edi;"
        "pop     esi;"
        "ret;"
        "loc_90D173:;"
        "add     dword ptr [esi+8], 8;"
        "pop     edi;"
        "pop     esi;"
        "ret;");
}
inline void lua_createtable(lua_State *l, int narr, int nhash = 0)
{
    reinterpret_cast<void (*)(lua_State *, int, int)>(&__lua_createtable)(l, narr, nhash);
}

Vector3f ToVector(lua_State *l, int index)
{
    Vector3f res;
    lua_pushvalue(l, index);
    lua_rawgeti(l, -1, 1);
    res.x = lua_tonumber(l, -1);
    lua_rawgeti(l, -1, 2);
    res.y = lua_tonumber(l, -1);
    lua_rawgeti(l, -1, 3);
    res.z = lua_tonumber(l, -1);
    lua_pop(l, 1);
    return res;
}

void PushVector(lua_State *l, Vector3f v)
{
    lua_createtable(l, 3);
    lua_pushnumber(l, v.x);
    lua_rawseti(l, -2, 1);
    lua_pushnumber(l, v.y);
    lua_rawseti(l, -2, 2);
    lua_pushnumber(l, v.z);
    lua_rawseti(l, -2, 3);
}

void ProjectVectors(lua_State *l, int index, void *camera)
{

    lua_pushvalue(l, index);
    lua_pushnil(l);
    while (lua_next(l, -2)) // -1 = value, -2 =  key, -3 = table
    {
        Vector3f v = ToVector(l, -1);
        
        lua_pop(l, 1);
    }

    lua_pop(l, 1);
}

int ProjectMultiple(lua_State *l)
{
    if (lua_gettop(l) != 2)
    {
        l->LuaState->Error(s_ExpectedButGot, __FUNCTION__, 2, lua_gettop(l));
    }

    Result<CUIWorldView> r = GetCScriptObject<CUIWorldView>(l, 1);
    void *worldview = r.object;
    if (worldview == nullptr)
    {
        lua_pushstring(l, r.reason);
        lua_error(l);
        return 0;
    }

    void *v = (void *)((int)worldview + 284);
    void *camera = reinterpret_cast<void *(__thiscall *)(void *)>(*(int *)v + 12)(v);

    return 1;
}