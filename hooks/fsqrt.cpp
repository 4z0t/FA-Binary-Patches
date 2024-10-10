#include "../asm.h"

asm(
    SECTION(0, 0x00452FC0)
    "fld dword ptr [esp+4];"
    "fsqrt;"
    "ret;"
    SECTION(1, 0x0040EAF0)
    "fld dword ptr [esp+4];"
    "fsqrt;"
    "ret;"
);