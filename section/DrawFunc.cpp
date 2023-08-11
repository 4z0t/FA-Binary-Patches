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

inline int DrawRect(
    float *v1,
    float *v2,
    unsigned int i,
    float f1,
    void *batcher,
    float *v3,
    void *heightmap,
    float f2)
{
    return reinterpret_cast<int (*)(float *, float *, unsigned int, float, void *, float *, void *, float)>(_DrawRect)(v1, v2, i, f1, batcher, v3, heightmap, f2);
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
        Texture FromSolidColor(unsigned int color)
        {
            Texture t;
            reinterpret_cast<void (*)(Texture *, unsigned int)>(0x4478C0)(&t, color);
            return t;
        }

        void __stdcall FromSolidColor(Texture *t, unsigned int color)
        {
            reinterpret_cast<void (*)(Texture *, unsigned int)>(0x4478C0)(t, color);
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
                "pop ebx;");
        }

        void __stdcall SetTexture(void *batcher, Texture *texture)
        {
            reinterpret_cast<void (*)(Texture *, void *)>(_SetTexture)(texture, batcher);
        }

        void _SetViewProjMatrix()
        {
            asm(
                "push ebx;"
                "mov ebx, [esp+0x0C];" // matrix
                "push ebx;"
                "mov ebx, [esp+0x0C];"// batcher
                "call 0x4385F0;"
                //"add esp, 4;"
                "pop ebx;");
        }

        void __stdcall SetViewProjMatrix(void *batcher, void *matrix)
        {
            reinterpret_cast<void (*)(void *, void *)>(_SetViewProjMatrix)(batcher, matrix);
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

    //"primbatcher"

    //"TAlphaBlendLinearSampleNoDepth"

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
    LuaState *ls = l->LuaState;

    int *batcher = *(int **)(((int *)g_WRenViewport) + 2135);
    if (batcher == nullptr)
    {
        WarningF("%s", "LOX");
        return 0;
    }

    float a[]{0, 8, 0};
    float b[]{8, 0, 0};
    float c[]{653.5, 18.77, 168.5};
    void *wldmap = IWldTerrainRes::GetWldMap();
    void *terrain = IWldTerrainRes::GetTerrainRes(wldmap);
    if (!terrain)
        return 0;
    void *map = IWldTerrainRes::GetMap(terrain);
    DebugLog("Here!");

    // int *device = Moho::D3D_GetDevice();
    //(*(void(__thiscall **)(int *))(*device + 4))(device);
    // Moho::SetupDevice(device, "primbatcher", "TAlphaBlendLinearSampleNoDepth");

    DebugLog("after setup");
    //  Moho::CPrimBatcher::ResetBatcher(batcher);
    DebugLog("after reset");
    DrawRect(a, b, 0xFFFFFF00, 0.03, batcher, c, map, 17.5);
    DebugLog("after draw");
    Moho::CPrimBatcher::FlushBatcher(batcher);
    DebugLog("after flush");
    //(*(void(__thiscall **)(int *))(*device + 4))(device);
    return 0;
}

void __thiscall CustomDraw(void *_this, void *batcher)
{
    void *wldmap = IWldTerrainRes::GetWldMap();
    void *terrain = IWldTerrainRes::GetTerrainRes(wldmap);
    if (!terrain)
        return;
    void *map = IWldTerrainRes::GetMap(terrain);
    if (!map)
        return;

    int *device = Moho::D3D_GetDevice();
    Moho::SetupDevice(device, "primbatcher", "TAlphaBlendLinearSampleNoDepth");

    int *camera = *(int **)((int)_this + 4);
    void *projmatrix = (*(void *(__thiscall **)(int *))(*camera + 8))(camera);
    Moho::CPrimBatcher::ResetBatcher(batcher);
    Moho::CPrimBatcher::SetViewProjMatrix(batcher, projmatrix);
    Moho::CPrimBatcher::Texture t;
    Moho::CPrimBatcher::FromSolidColor(&t, 0xFFFFFFFF);
    Moho::CPrimBatcher::SetTexture(batcher, &t);
    float a[]{0, 0, 8};
    float b[]{8, 0, 0};
    float c[]{653.5f, 18.77f, 168.5f};
    DrawRect(a, b, 0xFFFFFF00, 3.f, batcher, c, map, 17.5f);
    Moho::CPrimBatcher::FlushBatcher(batcher);
}

void CustomDrawEnter()
{
    asm(
        "push edi;"
        "mov ecx, esi;"
        "call %[CustomDraw];"
        //"add esp, 4;"
        "pop     edi;" // as done in original code
        "pop     esi;"
        "pop     ebx;"
        "mov     esp, ebp;"
        "jmp     0x86EF30;" // jump back
        :
        : [CustomDraw] "i"(CustomDraw)
        :);
}