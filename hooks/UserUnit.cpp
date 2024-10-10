#include "../asm.h"
#include "../define.h"

asm(
    SECTION(0, 0x008949FE)
    "push 1008;"
    SECTION(1, 0x008BF979)
    "jmp "QU(ExtendCtor)";"
    SECTION(2, 0x008BF994)
    "call "QU(ExtendDtor)";"
    SECTION(3, 0x0085E07C)
    "jmp "QU(ExtendRenderIcon)";"
    "nop;"
    "nop;"
);