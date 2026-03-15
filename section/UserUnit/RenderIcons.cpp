
#include "RenderIcons.h"
#include "CObject.h"

void ExtendCtor()
{
    asm("push ebp;"
        "call %[ExtendUserUnit];"
        "pop    edi;"
        "pop    esi;"
        "mov    eax, ebp;"
        "pop    ebp;"
        "jmp    0x008BF97E;"
        :
        : [ExtendUserUnit] "i"(ExtendUserUnitCtor)
        :);
}

void ExtendDtor()
{
    asm("push esi;"
        "push esi;"
        "call 0x8BF9B0;"
        "call %[ExtendUserUnitDtor];"
        "ret 4;"
        :
        : [ExtendUserUnitDtor] "i"(ExtendUserUnitDtor));
}

void ExtendRenderIcon()
{

    asm("lea  edi, [esp+0xC0-0x80];"
        "push edi;"
        "push [ebp+0xC];"
        "push [ebp+0x8];"
        "call %[ExtendRenderUserUnitIcon];"
        "mov     ecx, [esp+0xC0-0xC];"
        "jmp 0x0085E083;"
        :
        : [ExtendRenderUserUnitIcon] "i"(ExtendRenderUserUnitIcon)
        :);
}
