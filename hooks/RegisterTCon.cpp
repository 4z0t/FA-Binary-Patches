#include "../define.h"

asm(
  ".section h0; .set h0,0xBE8A50;"
  "jmp "QU(OnRegisterTCon)";"
);
