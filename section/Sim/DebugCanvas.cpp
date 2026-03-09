#include "DebugCanvas.h"

void Moho__CDebugCanvas__AddWireCoords(Vector3f *pos, /* Moho::CDebugCanvas */ void *canvas, Quaternion *a3, float a4)
{
    asm(
        "push %[a4];"
        "push %[a3];"
        "push %[canvas];"
        "call 0x450330;"
        :
        : "D"(pos), [canvas] "g"(canvas), [a3] "g"(a3), [a4] "g"(a4)
        :);
}