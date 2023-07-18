#include "include/moho.h"

void CheckPointer()
{
    register lua_State *l asm("eax");
    register void *ptr asm("ebx");
    lua_pushboolean(l, ptr == nullptr);
}

void SetStatCheck()
{
    asm(
        // mov    ebx, eax //pointer to our stat in other thread
        "mov     eax, [edi];" // pointer to lua state
        "cmp ebx, 0;"
        "je SetStatCheck_invalid;" // go to return 1
        "mov     esi, 3;"
        "push    esi;"
        "push    eax;"
        "call    0x90C740;" // lua_type;"
        "add     esp, 8;"
        "cmp     eax, esi;"
        "push    esi;"
        "jnz     0x6CCAAF;"
        "mov     [esp+0x54-0x44], edi;"
        "mov     [esp+0x54-0x40], esi;"
        "mov     ecx, [edi];"
        "push    ecx;"
        "call    0x90C740;" // lua_type;"
        "add     esp, 8;"
        "cmp     eax, esi;"
        "jz      0x6CCA65;"
        "push    offset 0xE01168;"
        "lea     ecx, [esp+0x54-0x44];"
        "jmp 0x6CCA60;" // LuaStackObjectTypeError
        "SetStatCheck_invalid:"
        "call %[CheckPointer];"
        "mov eax, 1;"
        "pop     edi;"
        "pop     esi;"
        "pop     ebx;"
        "mov     esp, ebp;"
        "pop     ebp;"
        "ret;"
        :
        : [CheckPointer] "i"(CheckPointer)
        :
        );
}
