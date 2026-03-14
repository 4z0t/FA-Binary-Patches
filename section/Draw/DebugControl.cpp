#include "DebugControl.h"

void asm__DebugControl()
{
    asm volatile(
        "call    0x00455480;"
        "add     esp, 0x10;"
        "lea     ecx, [esp + 0x2C];" // Top left vector3f
        "push    ecx;"
        "mov     ecx, [esp + 4 + 0x10];" // control
        "push    esi;"                   // batcher
        "call   %[_DrawDebugControl];"
        "jmp     0x0084DA69;"
        :
        : [_DrawDebugControl] "i"(_DrawDebugControl)
        :);
}