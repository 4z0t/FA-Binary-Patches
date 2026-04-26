#include "Networkstats.h"

void asm__NetworkstatsHook()
{
    /*
        ecx - string
        esi - column index
    */
    asm(
        "fstp    dword ptr [esp];"
        "mov     eax, [esp + 0x44 + 4];" // row
        "pushf;"
        "push    ecx;" // preserve these
        "push    edx;" // preserve these
        "push    esi;"
        "push    eax;"
        "call    %[PickColorForConnections];"
        "pop     edx;"
        "pop     ecx;"
        "popf;"
        "push    eax;" // color
        "jmp     0x0073EFC1;"
        :
        : [PickColorForConnections] "i"(PickColorForConnections)
        :);
}