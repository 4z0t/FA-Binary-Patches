
#pragma once
#include "Refaf/UI/Font.h"

namespace Refaf
{
    void __stdcall Font__Dtor(Font *font) asm("0x425860");

    SHARED
    {
        void *InternalCreate(FontRef * font, const char *font_name, int font_size);
    }
}