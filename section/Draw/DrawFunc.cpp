#include "DrawFunc.h"
#include "CObject.h"
#include "magic_classes.h"
#include "moho.h"
#include "utility.h"
#include <cmath>

#define NON_GENERAL_REG(var_) [var_] "g"(var_)

void _DrawRect(
    Vector3f *v1,
    Vector3f *v2,
    unsigned int color,
    float thickness,
    void *batcher,
    Vector3f *v3,
    void *heightmap,
    float f2)
{
    asm("push %[f2];"
        "push %[heightmap];"
        "push %[v3];"
        "push %[batcher];"
        "movss xmm0, %[thickness];"
        "call 0x00455480;"
        "add esp, 0x10;"
        :
        : "c"(v2),
          "D"(color),
          "a"(v1),
          [thickness] "m"(thickness),
          NON_GENERAL_REG(f2),
          NON_GENERAL_REG(batcher),
          NON_GENERAL_REG(v3),
          NON_GENERAL_REG(heightmap)
        : "xmm0");
}

float THICKNESS = 0.1;
unsigned int CIRCLE_COLOR = 0xFF00D800;
float SEGMENT_RAD = 0.1308996939f;
void _DrawCircle(
    void *batcher,
    Vector3f *pos,
    float radius,
    float thickness,
    uint32_t color,
    Vector3f *orientation)
{
    THICKNESS = thickness;
    CIRCLE_COLOR = color;
    asm("push %[radius];"
        "push %[pos];"
        "push %[batcher];"
        "call 0x00456200;"
        "add esp, 0xC;"
        :
        : "c"(orientation),
          NON_GENERAL_REG(batcher),
          NON_GENERAL_REG(pos),
          NON_GENERAL_REG(radius)
        : "eax");
}

unsigned int GetCircleColor()
{
    return CIRCLE_COLOR;
}

void __stdcall Moho::CPrimBatcher::SetTexture(void *batcher, Texture *texture)
{
    asm("call 0x4386A0;"
        :
        : "D"(batcher),
          "b"(texture)
        : "edx", "ecx", "eax");
}

void __stdcall Moho::CPrimBatcher::SetViewProjMatrix(void *batcher, void *matrix)
{
    asm("push %[matrix];"
        "call 0x438640;"
        :
        : "b"(batcher),
          NON_GENERAL_REG(matrix)
        : "edx", "eax");
}

void CustomDrawEnter()
{
    asm("push edi;"
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
SHARED float delta_frame;

void StoreFPS()
{
    asm("movss  xmm0, dword ptr [esp+0x78-0x68];"
        "movss ds:%[delta_frame], xmm0;"
        "jmp 0x8D1A8B;"
        :
        : [delta_frame] "i"(&delta_frame)
        :);
}

char *DRAW_WireBox(VMatrix4 *a1, /*CD3DPrimBatcher*/ void *batcher)
{
    char *__result;
    asm("call 0x00454680;"
        : "=a"(__result)
        : [a1] "a"(a1),
          [batcher] "S"(batcher)
        :);
    return __result;
}

char *DrawLine(Vertex *v1, void *batcher, Vertex *v2)
{
    char *__result;
    asm("push %[v2];"
        "call 0x004392C0;"
        : "=a"(__result)
        : [v1] "a"(v1),
          [batcher] "c"(batcher),
          [v2] "g"(v2)
        :);
    return __result;
}

char *DrawQuad(
    Vertex *a1,
    void *batcher,
    Vertex *a3,
    Vertex *a4,
    Vertex *a5)
{
    char *__result;
    asm("push %[a5];"
        "push %[a4];"
        "push %[a3];"
        "call 0x004389A0;"
        : "=a"(__result)
        : [a1] "a"(a1),
          [batcher] "c"(batcher),
          [a3] "g"(a3),
          [a4] "g"(a4),
          [a5] "g"(a5)
        :);
    return __result;
}

void *Moho::CPrimBatcher::ReleaseTexture(Moho::CPrimBatcher::Texture *t)
{
    void *__result;
    asm("call 0x004260B0;"
        : "=a"(__result)
        : [t] "a"(t)
        : "edx", "ecx");
    return __result;
}

void DrawQuad2(
    unsigned int color,
    Vector3f *a2,
    Vector3f *ecx0,
    Vector3f *esi0,
    void *batcher,
    Vector3f *a6)
{
    asm("push %[a6];"
        "push %[a5];"
        "call 0x00438DA0;"
        :
        : [color] "a"(color),
          [a2] "d"(a2),
          [ecx0] "c"(ecx0),
          [esi0] "S"(esi0),
          [a5] "g"(batcher),
          [a6] "g"(a6)
        :);
}