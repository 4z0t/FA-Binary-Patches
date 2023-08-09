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
    int *batcher,
    float *v3,
    void *heightmap,
    float f2)
{
    return reinterpret_cast<int (*)(float *, float *, unsigned int, float, int *, float *, void *, float)>(_DrawRect)(v1, v2, i, f1, batcher, v3, heightmap, f2);
}

void FlushBatcher(void *batcher)
{
    reinterpret_cast<void (*)(void *)>(0x0043A140)(batcher);
    asm("add esp,4;");
}

void ResetBatcher(void *batcher)
{
    *(char *)((int *)batcher + 285) = 0;
}

namespace Moho
{
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
    // ResetBatcher(batcher);
    DebugLog("after reset");
    DrawRect(a, b, 0xFFFFFF00, 0.03, batcher, c, map, 17.5);
    DebugLog("after draw");
    FlushBatcher(batcher);
    DebugLog("after flush");
    //(*(void(__thiscall **)(int *))(*device + 4))(device);
    return 0;
}

void CustomDraw(void* batcher)
{
    
}

void CustomDrawEnter()
{
    asm(
        "push edi;"
        "call %[CustomDraw];"
        "pop     edi;" // as done in original code
        "pop     esi;"
        "pop     ebx;"
        "mov     esp, ebp;"
        "jmp     0x86EF30;" // jump back
        :
        :[CustomDraw] "i" (CustomDraw)
        :
    );
}