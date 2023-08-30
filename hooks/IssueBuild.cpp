

#include "../define.h"
asm(
    ".section h0; .set h0,0x6F5F97;"
   // "lea edx, [esp+0x94];"
   "jmp "QU(IssueBuildMobileFix)";"
   "nop;"
   "nop;"
   "nop;"
   "nop;"

);

