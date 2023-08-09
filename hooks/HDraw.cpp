//

#include "../define.h"
asm(
    ".section h0; .set h0,0x86EF2B;"
    "jmp "QU(CustomDrawEnter)";"
);
