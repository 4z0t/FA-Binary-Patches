#include "include/moho.h"

void CheckUserUnit()
{
    asm(
        "push edi;"
        "mov eax, [esp+0x8];"
        "mov edi, [esp+0xC];"
        "call 0x00822B80;"
        "pop edi;"
        "ret;");
}

namespace Moho
{
    namespace UserUnit
    {
        inline float GetFractionComplete(void *unit)
        {
            return *((float *)unit + 44);
        }

        inline void *GetMeshInstance(void *unit)
        {
            return *((void **)unit + 11);
        }

        inline void* GetUserUnit(LuaObject * obj, LuaState *luaState)
        {
            return reinterpret_cast<void *(*)(LuaObject *, LuaState *)>(CheckUserUnit)(obj, luaState);
        }
    } // namespace UserUnit

    namespace MeshInstance
    {
        inline void UpdateInterpolatedTransform(void* mesh)
        {
            reinterpret_cast<void(__stdcall*)(void*)>(0x007DEC80)(mesh);
        }
    } // namespace MeshInstance
    

}

int GetInterpolatedPosition(lua_State *l)
{
    if (lua_gettop(l) != 1)
    {
        l->LuaState->Error(ExpectedButGot, __FUNCTION__, 1, lua_gettop(l));
    }
    LuaObject unitObject{l->LuaState, 1};
    //float *unit = (float *)Moho::UserUnit::GetUserUnit(&unitObject, l->LuaState);
    float *unit = (float *)reinterpret_cast<void *(*)(LuaObject *, LuaState *)>(CheckUserUnit)(&unitObject, l->LuaState);
    if (unit == nullptr)
        return 0;
    float *mesh = (float*)Moho::UserUnit::GetMeshInstance(unit);
    if (mesh == nullptr)
        return 0;
    Moho::MeshInstance::UpdateInterpolatedTransform(mesh);
    lua_pushnumber(l, mesh[34]);
    lua_pushnumber(l, mesh[35]);
    lua_pushnumber(l, mesh[36]);
    return 3;
}
// UI_Lua LOG(GetSelectedUnits()[1].GetInterpolatedPosition)
// UI_Lua LOG(GetSelectedUnits()[1].GetInterpolatedPosition())
// UI_Lua LOG(GetSelectedUnits()[1]:GetInterpolatedPosition())
// UI_Lua LOG(GetSelectedUnits()[1].GetInterpolatedPosition{})--crash
// UI_Lua LOG(GetSelectedUnits()[1].GetInterpolatedPosition(GetCamera"WorldCamera"))--crash

int GetFractionComplete(lua_State *l)
{
    if (lua_gettop(l) != 1)
    {
        l->LuaState->Error(ExpectedButGot, __FUNCTION__, 1, lua_gettop(l));
    }
    LuaObject unitObject{l->LuaState, 1};
    void *unit = reinterpret_cast<void *(*)(LuaObject *, LuaState *)>(CheckUserUnit)(&unitObject, l->LuaState);
    if (unit == nullptr)
        return 0;
    lua_pushnumber(l, Moho::UserUnit::GetFractionComplete(unit));
    return 1;
}
// UI_Lua LOG(GetSelectedUnits()[1].GetFractionComplete)
// UI_Lua LOG(GetSelectedUnits()[1].GetFractionComplete())
// UI_Lua LOG(GetSelectedUnits()[1]:GetFractionComplete())
// UI_Lua LOG(GetSelectedUnits()[1].GetFractionComplete{})--crash