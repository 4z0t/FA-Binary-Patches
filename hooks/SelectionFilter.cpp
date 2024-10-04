#include "../asm.h"
#include "../define.h"
asm(
  // SECTION(0,0x00863C22)
  //   "NOP;"
  //   "NOP;"
  //   "NOP;"
  //   "NOP;"
  //   "NOP;"
  // SECTION(0,0x00863A59)
  // "call "QU(HookSelection)";"
  // "nop;"
  // "nop;"
  SECTION(0,0x00863967)
  "jmp "QU(HookSelectionMerge)";"
  SECTION(1,0x0086398D)
  "call "QU(HookSelection)";"
  "nop;"
  "nop;"
  SECTION(2,0x0086390B)
  "jmp "QU(HookSelectionCondition)";"
  SECTION(3,0x00863927)
  "jmp "QU(HookSelectionCondition2)";"

);