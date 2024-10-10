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
    SECTION(2, 0x0050D150)
    "fld dword ptr [esp+4];"
    "fsqrt;"
    "fld1;"
    "fdivrp  st(1), st;"
    "ret;"
);