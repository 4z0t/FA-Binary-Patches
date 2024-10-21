
#include "UserUnits.h"
#include "CObject.h"
#include "moho.h"
#include "utility.h"

int get_session_user_entities(BaseVector<UserEntity *> *output, int a2,
                              Moho::struct_session_res3 *a3)
{
    int __result;
    asm("push %[a3];"
        "call 0x00503F80;"
        : "=a"(__result)
        : [output] "a"(output), [a2] "c"(a2), [a3] "g"(a3)
        : "edx");
    return __result;
}

BitSetGetResult *BitSetGet_(BitSetGetResult *result, BitSet *a2,
                            unsigned int a3)
{
    BitSetGetResult *__result;
    asm("call 0x006D3090;"
        : "=a"(__result)
        : [result] "a"(result), [a2] "d"(a2), [a3] "D"(a3)
        :);
    return __result;
}

void *CheckUserUnit(LuaObject *obj, LuaState *ls)
{
    void *result;
    asm("call 0x00822B80;" : "=a"(result) : [obj] "a"(obj), [ls] "D"(ls) :);

    return result;
}

namespace Moho
{
    namespace UserUnit_
    {
        inline float GetFractionComplete(void *unit) { return *((float *)unit + 44); }

        inline void *GetMeshInstance(void *unit) { return *((void **)unit + 11); }

        inline void *GetUserUnit(LuaObject *obj, LuaState *luaState)
        {
            return CheckUserUnit(obj, luaState);
        }
    } // namespace UserUnit_

    namespace MeshInstance
    {
        void __stdcall UpdateInterpolatedTransform(void *mesh) asm("0x007DEC80");
    } // namespace MeshInstance

} // namespace Moho
int GetInterpolatedPosition(lua_State *l)
{
    if (lua_gettop(l) != 1)
    {
        l->LuaState->Error(s_ExpectedButGot, __FUNCTION__, 1, lua_gettop(l));
    }
    Result<UserUnit> r = GetCScriptObject<UserUnit>(l, 1);
    if (r.IsFail())
    {
        lua_pushstring(l, r.reason);
        lua_error(l);
        return 0;
    }
    void *unit = r.object;
    if (unit == nullptr)
        return 0;

    float *mesh = (float *)Moho::UserUnit_::GetMeshInstance(unit);
    if (mesh == nullptr)
        return 0;

    Moho::MeshInstance::UpdateInterpolatedTransform(mesh);
    PushVector3f(l, {mesh[34], mesh[35], mesh[36]});
    return 1;
}
// for testing
// UI_Lua LOG(GetSelectedUnits()[1].GetInterpolatedPosition)
// UI_Lua LOG(GetSelectedUnits()[1].GetInterpolatedPosition())
// UI_Lua LOG(GetSelectedUnits()[1]:GetInterpolatedPosition())
// UI_Lua LOG(GetSelectedUnits()[1].GetInterpolatedPosition{})
// UI_Lua
// LOG(GetSelectedUnits()[1].GetInterpolatedPosition(GetCamera"WorldCamera"))

int GetFractionComplete(lua_State *l)
{
    if (lua_gettop(l) != 1)
    {
        l->LuaState->Error(s_ExpectedButGot, __FUNCTION__, 1, lua_gettop(l));
    }

    Result<UserUnit> r = GetCScriptObject<UserUnit>(l, 1);

    if (r.IsFail())
    {
        lua_pushstring(l, r.reason);
        lua_error(l);
        return 0;
    }
    void *unit = r.object;
    if (unit == nullptr)
        return 0;
    lua_pushnumber(l, Moho::UserUnit_::GetFractionComplete(unit));
    return 1;
}
// UI_Lua LOG(GetSelectedUnits()[1].GetFractionComplete)
// UI_Lua LOG(GetSelectedUnits()[1].GetFractionComplete())
// UI_Lua LOG(GetSelectedUnits()[1]:GetFractionComplete())
// UI_Lua LOG(GetSelectedUnits()[1].GetFractionComplete{})

UserUnitMethodReg UserUnitGetInterpolatedPosition{
    "GetInterpolatedPosition",
    "UserUnit:GetInterpolatedPosition()",
    GetInterpolatedPosition,
    s_UserUnit};

UserUnitMethodReg UserUnitGetFractionComplete{
    "GetFractionComplete",
    "UserUnit:GetFractionComplete()",
    GetFractionComplete,
    s_UserUnit};
