#include "include/moho.h"

void _DrawRect()
/*
    eax vec3_1
    ecx vec3_2
    edi int
    xmm0 float_1
    stack batcher
    stack vec3_3
    stack heightmap
    stack float_2
*/
{
    asm(
        "offset = 0x04;"
        "push ebp;"
        "mov ebp, esp;"
        "push ecx;"
        "push edi;"
        "mov ecx,   [ebp + 0x04 + 0x08];" // vec3_2
        "mov edi,   [ebp + 0x04 + 0x0C];" // int
        "movss xmm0,[ebp + 0x04 + 0x10];" // float_1
        "mov eax,   [ebp + 0x04 + 0x20];"  // float_2
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
    int i,
    float f1,
    int *batcher,
    float *v3,
    int *heightmap,
    float f2)
{
    return reinterpret_cast<int (*)(float *, float *, int, float, int *, float *, int *, float)>(_DrawRect)(v1, v2, i, f1, batcher, v3, heightmap, f2);
}
// UI_Lua DrawRect()
int LuaDrawRect(lua_State *l)
{
    LuaState *ls = l->LuaState;

    //   int *batcher = (int*)(0x112D6140);//(int *)(g_WRenViewport + 2135);
    int **batcher = (int **)(((int *)g_WRenViewport) + 2135);
    if (batcher == nullptr)
    {
        WarningF("%s", "LOX");
        return 0;
    }
    LogF("%p", batcher);
    LogF("%p", *batcher);
    float a[]{0, 0, 0};
    float b[]{255, 255, 0};
    float c[]{0, 255, 0};
    DrawRect(a, b, -65281, 5.0f, *batcher, c, 0, -1000.0f);
    return 0;
}