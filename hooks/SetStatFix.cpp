#include "../define.h"
asm(
  ".section h0; .set h0,0x6CCA2A;"
  "jmp "QU(SetStatCheck)";"
  "nop;"
  "nop;"
  "nop;"
  "nop;"
  "nop;"
  "nop;"
  ".section h1; .set h1,0x6CCA9B;"
  "mov eax ,1;"//return 1

  ".section h2; .set h2,0x6CCB2F;"
  "mov eax ,1;"//return 1

  ".section h3; .set h3,0x6CCADE;"
  "mov eax ,1;"//return 1
);
