

#include "../define.h"
asm(

    ".section h0; .set h0,0x6F5B98;"
    "cmp eax, 5;"
    ".section h2; .set h2,0x6F5B9E;"
    "push 5;"

    ".section h1; .set h1,0x6F5F97;"
   // "lea edx, [esp+0x94];"
   "jmp "QU(IssueBuildMobileFix)";"
   "nop;"
   "nop;"
   "nop;"
   "nop;"

);

