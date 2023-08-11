//

#include "../define.h"
asm(
    ".section h0; .set h0,0x86EF2B;"
    "jmp "QU(CustomDrawEnter)";"
);


// asm(
//     ".section h0; .set h0,0x854C50;"
//     "jmp "QU(CustomDrawEnter)";"
// );
