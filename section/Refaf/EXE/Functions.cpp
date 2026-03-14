#include "Refaf/EXE/Functions.h"

void *Refaf::InternalCreate(FontRef *font, const char *name, int size)
{
    void *__result;
    asm(
        "push %[name];"
        "push %[font];"
        "call 0x00425290;"
        "add esp, 0x8"
        : "=a"(__result)
        : "c"(size), [name] "g"(name), [font] "g"(font)
        :);
    return __result;
}