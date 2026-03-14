#include "DrawFunc.h"
#include "CObject.h"
#include "magic_classes.h"
#include "moho.h"
#include "utility.h"
#include <cmath>

extern void *default_font asm("0x010C4284");

SHARED void __thiscall _DrawDebugControl(void *control, void *batcher, Vector3f *tl);