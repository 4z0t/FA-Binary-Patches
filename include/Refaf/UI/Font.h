#pragma once
#include "CountedObject.h"

namespace Refaf
{
    class Font;

    void __stdcall Font__Dtor(Font *font) asm("0x425860");

    class Font : public CountedObject
    {
    };

    class FontRef;

    SHARED void *InternalCreate(FontRef *font, const char *font_name, int font_size);

    class FontRef
    {
        Font *font;

    public:
        FontRef() : font(nullptr)
        {
        }

        void Create(const char *font_name, int font_size)
        {
            if (font != nullptr)
            {
                font->Release();
                font = nullptr;
            }
            InternalCreate(this, font_name, font_size);
        }

        Font *Get() { return font; }

        ~FontRef()
        {
            if (font != nullptr)
            {
                font->Release();
                font = nullptr;
            }
        }
    };

    static_assert(sizeof(FontRef) == 4);
}