#include "../define.h"
asm(
    ".section h0; .set h0,0x86EF2B;"
    "jmp " QU(CustomDrawEnter) ";"

    ".section h1; .set h1,0x456264;"
    "subss xmm0, ds:"QU(THICKNESS)";"

    ".section h2; .set h2,0x4562CD;"
    "addss xmm0, ds:"QU(THICKNESS)";"

    ".section h3; .set h3,0x4561A8;"
    "jmp "QU(_SetColor)";"
);
