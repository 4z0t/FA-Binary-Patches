#pragma once
#include "Refaf/Utility/Pad.h"
#include "CountedObject.h"

namespace Refaf
{
    class Font;

    class Font : public CountedObject
    {
        Pad<0x4C> pad;
    };

    class FontRef;

    class FontRef
    {
        Font *font;

        void Release();

    public:
        FontRef();

        void Create(const char *font_name, int font_size);

        Font *Get() const { return font; }

        ~FontRef();
    };

    // 0x00425373
    static_assert(sizeof(Font) == 0x54);
    static_assert(sizeof(FontRef) == 4);
}