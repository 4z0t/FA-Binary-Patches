#include "Refaf/EXE/Functions.h"

void *Refaf::FontInternalCreate(FontRef *font, const char *name, int size)
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

void *Refaf::FontInternalRender2D(const Math::Vector2f *pos, void *batcher, Font *font, const char *chText, int color)
{
    void *__result;
    asm(
        "push %[color];"
        "push %[chText];"
        "push %[font];"
        "call 0x00426580;"
        : "=a"(__result)
        : "a"(pos), "d"(batcher), [font] "g"(font), [chText] "g"(chText), [color] "g"(color)
        :);
    return __result;
}