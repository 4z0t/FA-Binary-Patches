#include "include/moho.h"

// 455480
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
        "mov eax, [esp+  0x04];" // vec3_1
        "mov [esp+  0x04], ecx;"
        "mov ecx, [esp+ 0x08];" // vec3_2
        "mov [esp+  0x08], edi;"
        "mov edi, [esp+ 0x0C];"    // int
        "movss xmm0, [esp+ 0x10];" // float_1

        
        "add esp, 0x10;"
        "call 0x00455480;"
        "sub esp, 0x10;"
        "mov ecx, [esp+  0x04];"
        "mov edi, [esp+  0x08];");
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

int LuaDrawRect(lua_State *l)
{
    LuaState ls = l->LuaState;

    // int *batcher = (int *)(g_WRenViewport + 2135);
    // if (batcher == nullptr)
    // {
    //     return 0;
    // }
    float a[]{0, 0, 0};
    float b[]{255, 255, 0};
    float c[]{0, 255, 0};
    DrawRect(a, b, -65281, 5.0f, 0, c, 0, -1000);
    return 0;
}