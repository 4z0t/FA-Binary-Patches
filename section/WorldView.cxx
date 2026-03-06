#include "CObject.h"
#include "magic_classes.h"
#include "moho.h"
#include "utility.h"

float Dot(const Vector4f &a, const Vector4f &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Vector2f Project(const float *camera, const Vector3f &v)
{
    Vector2f dest;
    const VMatrix4 &viewProjection = GetField<const VMatrix4>(camera, 0x9C);
    const VMatrix4 &viewport = GetField<const VMatrix4>(camera, 0x284);
    Vector4f v0 = {v.x, v.y, v.z, 1.0f};
    Vector4f v1 = {viewProjection.d[0].d[0], viewProjection.d[1].d[0], viewProjection.d[2].d[0], viewProjection.d[3].d[0]};
    Vector4f v2 = {viewProjection.d[0].d[1], viewProjection.d[1].d[1], viewProjection.d[2].d[1], viewProjection.d[3].d[1]};
    Vector4f v3 = {viewProjection.d[0].d[3], viewProjection.d[1].d[3], viewProjection.d[2].d[3], viewProjection.d[3].d[3]};

    float inv = Dot(v0, v3);
    dest.x = viewport.d[3].d[2] * (Dot(v0, v1) / inv + 1.0f) * 0.5f + viewport.d[3].d[0];
    dest.y = -viewport.d[3].d[3] * (Dot(v0, v2) / inv + 1.0f) * 0.5f + viewport.d[3].d[3] + viewport.d[3].d[1];
    return dest;
}

void ProjectVectors(lua_State *l, int index, float *camera)
{
    const char *t = (const char *)lua_topointer(l, index);
    uint32_t asize;
    uint8_t hbits;
    GetTableAH((void *)t, &asize, &hbits);
    lua_createtable(l, asize, hbits); // result table
    lua_pushvalue(l, index);          // input vectors
    lua_pushnil(l);
    while (lua_next(l, -2)) // -1 = value, -2 =  key, -3 = table, -4 = result table
    {
        Vector3f v = ToVector(l, -1);
        Vector2f p = Project(camera, v);
        lua_pushvalue(l, -2); // key
        PushVector2f(l, p);   // value
        lua_rawset(l, -6);
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
    if (!lua_istable(l, 2))
    {
        luaL_argerror(l, 2, "table expected.");
    }
    Result<CUIWorldView> r = GetCScriptObject<CUIWorldView>(l, 1);
    if (r.IsFail())
    {
        lua_pushstring(l, r.reason);
        lua_error(l);
        return 0;
    }
    void *worldview = r.object;
    if (worldview == nullptr)
        return 0;
    void *camera = (void *)(*(int(__thiscall **)(int))(*(int *)((int)worldview + 284) + 12))((int)worldview + 284);
    if (camera == nullptr)
        return 0;
    float *geomcamera = (float *)(*(int(__thiscall **)(void *))(*(int *)camera + 8))(camera);
    ProjectVectors(l, 2, geomcamera);
    return 1;
}

// UI_Lua reprsl(import("/lua/ui/game/worldview.lua").viewLeft:GetScreenPos(GetSelectedUnits()[1]))
// UI_Lua reprsl(import("/lua/ui/game/worldview.lua").viewLeft.ProjectMultiple(import("/lua/maui/text.lua").Text(GetFrame(0)),{{1,2,3}}))
// UI_Lua reprsl(import("/lua/ui/game/worldview.lua").viewLeft:ProjectMultiple({{1,2,3}}))
// UI_Lua reprsl(import("/lua/ui/game/worldview.lua").viewLeft:ProjectMultiple({GetSelectedUnits()[1]:GetPosition()}))
// UI_Lua reprsl(import("/lua/ui/game/worldview.lua").viewLeft.ProjectMultiple())
// UI_Lua reprsl(import("/lua/ui/game/worldview.lua").viewLeft.ProjectMultiple({},{}))

using WorldViewMethodReg = RegFunc<0xF59690, 0x00E491E8, 0x00F8D88C>;

WorldViewMethodReg WorldViewProjectMultiple{
    "ProjectMultiple",
    "WorldView:ProjectMultiple(vectors)",
    ProjectMultiple,
    "CUIWorldView"};