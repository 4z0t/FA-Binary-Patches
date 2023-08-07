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

//#define ui_Terrain GDecl(0x010A6438, void *)
#define ui_WldMap GDecl(0x010C4F58, void *)

void FlushBatcher(void *batcher)
{
    reinterpret_cast<void (*)(void *)>(0x0043A140)(batcher);
    asm("add esp, 4;");
}

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

// UI_Lua DrawRect()
int LuaDrawRect(lua_State *l)
{
    LuaState *ls = l->LuaState;

    //   int *batcher = (int*)(0x112D6140);//(int *)(g_WRenViewport + 2135);
    int *batcher = *(int **)(((int *)g_WRenViewport) + 2135);
    if (batcher == nullptr)
    {
        WarningF("%s", "LOX");
        return 0;
    }
    // LogF("%p", batcher);
    // LogF("%p", *batcher);
    float a[]{0, 0, 8};
    float b[]{8, 0, 0};
    float c[]{653.5, 18.77, 168.5};
    void *wldmap = IWldTerrainRes::GetWldMap();
    void *terrain = IWldTerrainRes::GetTerrainRes(wldmap);
    if (!terrain)
        return 0;
    void *map = IWldTerrainRes::GetMap(terrain);
    LogF("%s", "Here!");
    DrawRect(a, b, 0xFF00FF00, 0.03, batcher, c, map, 17.5);
    // FlushBatcher(batcher);
    return 0;
}