#include "../define.h"
#define REP0(X)
#define REP1(X) X
#define REP2(X) REP1(X) X
#define REP3(X) REP2(X) X
#define REP4(X) REP3(X) X
#define REP5(X) REP4(X) X
#define REP6(X) REP5(X) X
#define REP7(X) REP6(X) X
#define REP8(X) REP7(X) X
#define REP9(X) REP8(X) X
#define REP10(X) REP9(X) X
asm(
  ".section h0; .set h0,0x6CED12;"
  "mov dword ptr [0x10B6FA8], "QU(GiveNukeSiloAmmo)";"
  ".section h1; .set h1,0x5CF618;"
  REP8("nop;")
  ".section h2; .set h2,0x5CEDA1;"
  REP8("nop;")
);