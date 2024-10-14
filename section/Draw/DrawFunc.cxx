#include "DrawFunc.h"
#include "CObject.h"
#include "magic_classes.h"
#include "moho.h"
#include "utility.h"
#include <cmath>

void DrawRect(
    Vector3f v1,
    Vector3f v2,
    uint32_t color,
    float f1,
    void *batcher,
    Vector3f v3,
    void *heightmap,
    float f2)
{
    return _DrawRect(&v1, &v2, color, f1, batcher, &v3, heightmap, f2);
}

float THICKNESS = 0.1;
extern unsigned int CIRCLE_COLOR;

namespace Moho
{
    void Import(lua_State *l, const char *filename)
    {
        lua_getglobal(l, "import");
        lua_pushstring(l, filename);
        lua_call(l, 1, 1);
    }

    void *GetWorldCamera(void *worldview)
    {
        int *camera = *(int **)((int)worldview + 4);
        void *projmatrix = (*(void *(__thiscall **)(int *))(*camera + 8))(camera);
        return projmatrix;
    }
    namespace CPrimBatcher
    {
        void ResetBatcher(void *batcher)
        {
            *(char *)((int *)batcher + 285) = 0;
        }

        SharedPtr<Texture> FromSolidColor(unsigned int color)
        {
            SharedPtr<Texture> t;
            FromSolidColor(&t, color);
            return t;
        }

    } // namespace CPrimBatcher

    void SetupDevice(int *device, const char *target, const char *mode)
    {
        (*(void(__thiscall **)(int *, const char *))(*device + 80))(device, target);
        (*(void(__thiscall **)(int *, const char *))(*device + 84))(device, mode);
    }

    float GetLODMetric(float *camera, const Vector3f &v)
    {
        return camera[169] * v.x + camera[170] * v.y + camera[171] * v.z + camera[172];
    }
} // namespace Moho

namespace IWldTerrainRes
{
    void *GetWldMap()
    {
        return (void *)g_CWldSession->map;
    }

    void *GetTerrainRes(void *wldmap)
    {
        return (void *)*((int *)wldmap + 1);
    }

    void *GetMap(void *wldTerrainRes)
    {
        return (void *)*((int *)wldTerrainRes + 1);
    }

} // namespace  IWldTerrainRes
bool is_in_render_world = false;

void *_worldview = nullptr;
// UI_Lua DrawRect()
int LuaDrawRect(lua_State *l)
{
    int *batcher = *(int **)(((int *)g_WRenViewport) + 2135);
    if (batcher == nullptr || _worldview == nullptr)
    {
        return 0;
    }
    if (!is_in_render_world)
    {
        luaL_error(l, "Attempt to call DrawRect outside of OnRenderWorld");
        return 0;
    }

    Vector3f pos = ToVector(l, 1);
    float size = luaL_checknumber(l, 2);
    const char *s = lua_tostring(l, 3);
    float thickness = luaL_optnumber(l, 4, 0.15);
    uint32_t color;
    if (!Moho::TryConvertToColor(s, color))
    {
        luaL_error(l, s_UnknownColor, s);
        return 0;
    }
    Vector3f orientation{0, 1, 0};
    float lod = Moho::GetLODMetric((float *)Moho::GetWorldCamera(_worldview), pos);
    float thick = std::max(thickness / lod, 2.f);
    Vector3f a{0, 0, size};
    Vector3f b{size, 0, 0};
    DrawRect(a, b, color, thick * lod, batcher, pos, nullptr, -10000);
    return 0;
}
static UIRegFunc DrawRectReg{"UI_DrawRect", "UI_DrawRect(pos:vector, size:float, color:string, thickness?=0.15:float)", LuaDrawRect};

int LuaDrawCircle(lua_State *l)
{
    int *batcher = *(int **)(((int *)g_WRenViewport) + 2135);
    if (batcher == nullptr || _worldview == nullptr)
    {
        return 0;
    }
    if (!is_in_render_world)
    {
        luaL_error(l, "Attempt to call DrawCircle outside of OnRenderWorld");
        return 0;
    }
    Vector3f pos = ToVector(l, 1);
    float r = luaL_checknumber(l, 2);
    const char *s = lua_tostring(l, 3);
    float thickness = luaL_optnumber(l, 4, 0.15);
    uint32_t color;
    if (!Moho::TryConvertToColor(s, color))
    {
        luaL_error(l, s_UnknownColor, s);
        return 0;
    }
    Vector3f orientation{0, 1, 0};
    float lod = Moho::GetLODMetric((float *)Moho::GetWorldCamera(_worldview), pos);
    float a = std::max(thickness / lod, 2.f);
    _DrawCircle(batcher, &pos, r, lod * a, color, &orientation);

    return 0;
}

static UIRegFunc DrawCircleReg{"UI_DrawCircle", "UI_DrawCircle(pos:vector, radius:float, color:string, thickness?=0.15:float)", LuaDrawCircle};

int LuaDrawBox(lua_State *l)
{
    int *batcher = *(int **)(((int *)g_WRenViewport) + 2135);
    if (batcher == nullptr || _worldview == nullptr)
    {
        return 0;
    }
    if (!is_in_render_world)
    {
        luaL_error(l, "Attempt to call DrawBox outside of OnRenderWorld");
        return 0;
    }

    VMatrix4 matrix = {
        128, 128, 128, 1.0, // x,y,z
        0, 0, 0, 1.0,       //
        0, 0, 0, 1.0,       //
        10, 10, 10, 0       // sizex, sizey, sizez
    };

    DRAW_WireBox(&matrix, batcher);
    return 0;
}

static UIRegFunc DrawBoxReg{"UI_DrawBox", "", LuaDrawBox};

int LuaDrawLine(lua_State *l)
{
    int *batcher = *(int **)(((int *)g_WRenViewport) + 2135);
    if (batcher == nullptr || _worldview == nullptr)
    {
        return 0;
    }
    if (!is_in_render_world)
    {
        luaL_error(l, "Attempt to call DrawLine outside of OnRenderWorld");
        return 0;
    }

    Vertex v1{Vector3f{256, 128, 256}, 0xFFFF00FF, 0, 0};
    Vertex v2{Vector3f{0, 128, 256}, 0xFFFF0000, 0, 1};
    Vertex v3{Vector3f{0, 128, 0}, 0xFF0000FF, 1, 1};
    Vertex v4{Vector3f{256, 128, 0}, 0xFF00FF00, 1, 0};
    // DrawLine(&v1, batcher, &v2);
    DrawQuad(&v1, batcher, &v2, &v3, &v4);
    return 0;
}

static UIRegFunc DrawLineReg{"UI_DrawLine", "", LuaDrawLine};

SHARED float delta_frame = 0.1;
// offset +284 from CUIWorldView
void __thiscall CustomDraw(void *_this, void *batcher)
{
    // void *wldmap = IWldTerrainRes::GetWldMap();
    // void *terrain = IWldTerrainRes::GetTerrainRes(wldmap);
    // if (!terrain)
    //     return;
    // void *map = IWldTerrainRes::GetMap(terrain);
    // if (!map)
    //     return;

    _worldview = _this;
    CUIWorldView *view = (CUIWorldView *)((int)_worldview - 284);

    if (!view->GetCustomRenderingEnabled())
        return;

    // LogF("%p", *(int *)(0x010A6470));//gamesession
    // LogF("%p", *(int *)((int)worldview - 284 + 532));

    LuaState *state = *(LuaState **)((int)g_CUIManager + 48);
    lua_State *l = state->m_state;
    LuaObject *worldviewTable = (LuaObject *)((int)_worldview - 284 + 32);
    // Moho::Import(l, "/lua/ui/game/gamemain.lua");
    worldviewTable->PushStack(l);
    lua_pushstring(l, "OnRenderWorld");
    lua_gettable(l, -2);
    if (!lua_isfunction(l, -1))
    {
        // pop worldviewTable and the value under 'OnRenderWorld' key
        lua_pop(l, 2);
        return;
    }
    int *device = Moho::D3D_GetDevice();
    Moho::SetupDevice(device, "primbatcher", "TAlphaBlendLinearSampleNoDepth");
    Moho::CPrimBatcher::ResetBatcher(batcher);
    Moho::CPrimBatcher::SetViewProjMatrix(batcher, Moho::GetWorldCamera(_worldview));
    SharedPtr<Moho::CPrimBatcher::Texture> t;
    Moho::CPrimBatcher::FromSolidColor(&t, 0xFFFFFFFF);
    Moho::CPrimBatcher::SetTexture(batcher, &t);

    is_in_render_world = true;
    lua_pushvalue(l, -2);
    lua_pushnumber(l, delta_frame);
    if (lua_pcall(l, 2, 0))
    {
        WarningF("%s", lua_tostring(l, -1));
    }
    lua_pop(l, 1);
    is_in_render_world = false;
    t.Release();
    Moho::CPrimBatcher::FlushBatcher(batcher);
    _worldview = nullptr;
}

int SetCustomRender(lua_State *l)
{
    if (lua_gettop(l) != 2)
    {
        l->LuaState->Error(s_ExpectedButGot, __FUNCTION__, 2, lua_gettop(l));
    }
    Result<CUIWorldView> r = GetCScriptObject<CUIWorldView>(l, 1);
    if (r.IsFail())
    {
        lua_pushstring(l, r.reason);
        lua_error(l);
        return 0;
    }
    CUIWorldView *worldview = r.object;
    if (worldview == nullptr)
        return 0;

    bool state = lua_toboolean(l, 2);
    worldview->SetCustomRenderingEnabled(state);
    return 0;
}

using WorldViewMethodReg = UIRegFuncT<0x00E491E8, 0x00F8D88C>;

WorldViewMethodReg WorldViewSetCustomRender{
    "SetCustomRender",
    "WorldView:SetCustomRender(bool)",
    SetCustomRender,
    "CUIWorldView"};
