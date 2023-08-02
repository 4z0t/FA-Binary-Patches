#include "../define.h"
asm(
    ".section h0; .set h0,0x4A29B4;"
    "mov     eax, offset "QU(EXCEPTION_ILLEGAL_INSTRUCTION)";"
);

