#include "../asm.h"

asm(
    SECTION(0, 0x00452FC0) // sqrt
    "fld dword ptr [esp+4]; fsqrt; ret;"
    SECTION(1, 0x0040EAF0) // sqrt
    "fld dword ptr [esp+4]; fsqrt; ret;"
    SECTION(2, 0x0050D150) // inverse sqrt
    "fld dword ptr [esp+4]; fsqrt; fld1; fdivrp  st(1), st; ret;"
    SECTION(3, 0x005734E0) // sin
    "fld dword ptr [esp+4]; fsin; ret;"
    SECTION(4, 0x005734D0) // cos
    "fld dword ptr [esp+4]; fcos; ret;"
    SECTION(5, 0x0040DAB0) // abs
    "fld dword ptr [esp+4]; fabs; ret;"
    SECTION(6, 0x004E9DF0) // atan2
    "fld dword ptr [esp+4]; fld dword ptr [esp+8]; fpatan; ret;"
);