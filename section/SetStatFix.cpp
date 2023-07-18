#include "include/moho.h"

void CheckPointer()
{
    register lua_State *l asm("eax");
    register void *ptr asm("ebx");
    lua_pushboolean(l, ptr != nullptr);
}

void SetStatCheck()
{
    asm(
        // mov    ebx, eax //pointer to our stat in other thread
        "mov     eax, [edi];" // pointer to lua state
        "call %[CheckPointer];"
        "cmp ebx, 0;"
        "jne 0x6CCA2C;" // jump at "mov     esi, 3"

        "jmp 0x6CCA9B;"//go to return 1
        :
        : [CheckPointer] "i"(CheckPointer)
        :
    );
}
