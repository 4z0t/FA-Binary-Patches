#include "../define.h"
asm(
    ".section h0; .set h0,0x789988;"
    "call "QU(_SetAlpha)";"

    ".section h1; .set h1,0x789972;"
    "call 0x90CA40;" //call lua_toboolean

    // ".section h2; .set h2,0x7899E0;"
    // "movss eax, xmm0;"
    // "push eax;"
    // "call "QU(_SetAlpha)";"
);