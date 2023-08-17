#include "include/moho.h"

void _DrawRect()
/*
    eax vec3_1
    ecx vec3_2
    edi int color?
    xmm0 float_1 //thickness
    stack batcher
    stack vec3_3
    stack heightmap
    stack float_2
*/
{
    asm(
        "push ebp;"
        "mov ebp, esp;"
        "push ecx;"
        "push edi;"
        "mov ecx,   [ebp + 0x04 + 0x08];" // vec3_2
        "mov edi,   [ebp + 0x04 + 0x0C];" // int
        "movss xmm0,[ebp + 0x04 + 0x10];" // float_1
        "mov eax,   [ebp + 0x04 + 0x20];" // float_2
        "push eax;"
        "mov eax,   [ebp + 0x04 + 0x1C];" // heightmap
        "push eax;"
        "mov eax,   [ebp + 0x04 + 0x18];" // vec3_3
        "push eax;"
        "mov eax,   [ebp + 0x04 + 0x14];" // batcher
        "push eax;"
        "mov eax,   [ebp + 0x04 + 0x04];" // vec3_1
        "call 0x00455480;"
        "add esp, 0x10;"
        "pop edi;"
        "pop ecx;"
        "pop ebp;");
}

int DrawRect(
    Vector3f v1,
    Vector3f v2,
    unsigned int i,
    float f1,
    void *batcher,
    Vector3f v3,
    void *heightmap,
    float f2)
{
    return reinterpret_cast<int (*)(Vector3f *, Vector3f *, unsigned int, float, void *, Vector3f *, void *, float)>(_DrawRect)(&v1, &v2, i, f1, batcher, &v3, heightmap, f2);
}

float THICKNESS = 0.1;
extern unsigned int CIRCLE_COLOR;
void _DrawCircle(void *batcher, Vector3f *pos, float radius, float thickness, uint32_t color, Vector3f *orientation)
{
    THICKNESS = thickness;
    CIRCLE_COLOR = color;
    asm(
        "push %[radius];"
        "push %[pos];"
        "push %[batcher];"
        "call 0x00456200;"
        :
        : "c"(orientation), [batcher] "g"(batcher), [pos] "g"(pos), [radius] "g"(radius)
        : "eax");
}
namespace Moho
{
    namespace CPrimBatcher
    {
        void FlushBatcher(void *batcher)
        {
            reinterpret_cast<void (*)(void *)>(0x0043A140)(batcher);
            asm("add esp,4;");
        }

        void ResetBatcher(void *batcher)
        {
            *(char *)((int *)batcher + 285) = 0;
        }

        struct Texture
        {
            int a;
            int b;
        };
        void __stdcall FromSolidColor(Texture *t, unsigned int color)
        {
            reinterpret_cast<void *(*)(Texture *, unsigned int)>(0x4478C0)(t, color);
        }

        Texture FromSolidColor(unsigned int color)
        {
            Texture t;
            FromSolidColor(&t, color);
            return t;
        }

        void _SetTexture()
        {
            asm(
                "push ebx;"
                "push edi;"
                "mov ebx, [esp+0x0C];"
                "mov edi, [esp+0x10];"
                "call 0x4386A0;"
                "pop edi;"
                "pop ebx;"
                :
                :
                : "edx");
        }

        void __stdcall SetTexture(void *batcher, Texture *texture)
        {
            reinterpret_cast<void *(*)(Texture *, void *)>(_SetTexture)(texture, batcher);
        }

        void _SetViewProjMatrix()
        {
            asm(
                "push ebx;"
                "mov ebx, [esp+0xC];" // matrix
                "push ebx;"
                "mov ebx, [esp+0xC];" // batcher
                "call 0x438640;"
                "pop ebx;"
                :
                :
                : "edx");
        }

        void __stdcall SetViewProjMatrix(void *batcher, void *matrix)
        {
            reinterpret_cast<void *(*)(void *, void *)>(_SetViewProjMatrix)(batcher, matrix);
        }
    } // namespace CPrimBatcher

    int *D3D_GetDevice()
    {
        return reinterpret_cast<int *(*)()>(0x00430590)();
    }

    void SetupDevice(int *device, const char *target, const char *mode)
    {
        (*(void(__thiscall **)(int *, const char *))(*device + 80))(device, target);
        (*(void(__thiscall **)(int *, const char *))(*device + 84))(device, mode);
    }

    bool TryConvertToColor(const char *s, uint32_t &color)
    {
        return reinterpret_cast<bool(__cdecl *)(const char *, uint32_t *)>(0x4B2B90)(s, &color);
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

#define DebugLog(_s) LogF("%s", (_s))

// UI_Lua DrawRect()
int LuaDrawRect(lua_State *l)
{
    int *batcher = *(int **)(((int *)g_WRenViewport) + 2135);
    if (batcher == nullptr)
    {
        return 0;
    }
    float x, y, z;
    const char *s;
    x = luaL_checknumber(l, 1);
    y = luaL_checknumber(l, 2);
    z = luaL_checknumber(l, 3);
    s = lua_tostring(l, 4);
    uint32_t color;
    if (!Moho::TryConvertToColor(s, color))
    {
        lua_pushstring(l, "unknown color");
        lua_error(l);
        return 0;
    }
    Vector3f a{0, 0, 8};
    Vector3f b{8, 0, 0};
    Vector3f c{x, y, z};
    DrawRect(a, b, color, 1.f, batcher, c, nullptr, -10000);
    Moho::CPrimBatcher::FlushBatcher(batcher);
    return 0;
}

int LuaDrawCircle(lua_State *l)
{
    int *batcher = *(int **)(((int *)g_WRenViewport) + 2135);
    if (batcher == nullptr)
    {
        return 0;
    }
    float x, y, z, r;
    const char *s;
    x = luaL_checknumber(l, 1);
    y = luaL_checknumber(l, 2);
    z = luaL_checknumber(l, 3);
    r = luaL_checknumber(l, 4);
    s = lua_tostring(l, 5);
    uint32_t color;
    if (!Moho::TryConvertToColor(s, color))
    {
        lua_pushstring(l, "unknown color");
        lua_error(l);
        return 0;
    }

    Vector3f pos{x, y, z};
    Vector3f orientation{0, 1, 0};
    _DrawCircle(batcher, &pos, r, 0.2, color, &orientation);

    Moho::CPrimBatcher::FlushBatcher(batcher);
    return 0;
}

void __thiscall CustomDraw(void *_this, void *batcher)
{
    // void *wldmap = IWldTerrainRes::GetWldMap();
    // void *terrain = IWldTerrainRes::GetTerrainRes(wldmap);
    // if (!terrain)
    //     return;
    // void *map = IWldTerrainRes::GetMap(terrain);
    // if (!map)
    //     return;

    LuaState *state = *(LuaState **)((int)g_CUIManager + 48);
    lua_State *l = state->m_state;

    lua_pushstring(l, "import");
    lua_gettable(l, -10001);
    lua_pushstring(l, "/lua/ui/game/gamemain.lua");
    lua_call(l, 1, 1);
    lua_pushstring(l, "OnRenderWorld");
    lua_rawget(l, -2);
    if (lua_type(l, -1) != 7)
    {
        WarningF("%s", "OnRenderWorld not a function");
        return;
    }
    int *device = Moho::D3D_GetDevice();
    Moho::SetupDevice(device, "primbatcher", "TAlphaBlendLinearSampleNoDepth");

    int *camera = *(int **)((int)_this + 4);
    void *projmatrix = (*(void *(__thiscall **)(int *))(*camera + 8))(camera);
    Moho::CPrimBatcher::ResetBatcher(batcher);
    Moho::CPrimBatcher::SetViewProjMatrix(batcher, projmatrix);
    Moho::CPrimBatcher::Texture t;
    Moho::CPrimBatcher::FromSolidColor(&t, 0xFFFFFFFF);
    Moho::CPrimBatcher::SetTexture(batcher, &t);
    lua_call(l, 0, 0);
    // Vector3f a{0, 0, 8};
    // Vector3f b{8, 0, 0};
    Vector3f pos{653.5f, 18.77f, 168.5f};
    Vector3f orientation{0, 1, 0};
    // DrawRect(a, b, 0xFFFFFF00, 3.f, batcher, c, nullptr, -10000);
    _DrawCircle(batcher, &pos, 100, 0.2, 0xFFFF0000, &orientation);
    Moho::CPrimBatcher::FlushBatcher(batcher);
}

void CustomDrawEnter()
{
    asm(
        "push edi;"
        "mov ecx, esi;"
        "call %[CustomDraw];"
        "pop     edi;" // as done in original code
        "pop     esi;"
        "pop     ebx;"
        "mov     esp, ebp;"
        "jmp     0x86EF30;" // jump back
        :
        : [CustomDraw] "i"(CustomDraw)
        :);
}