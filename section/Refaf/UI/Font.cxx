#include "Refaf/UI/Font.h"
#include "Refaf/EXE/Functions.h"

namespace Refaf
{

    FontRef::FontRef()
        : font{nullptr}
    {
    }

    void FontRef::Release()
    {
        if (font != nullptr)
        {
            font->Release();
            font = nullptr;
        }
    }

    FontRef::~FontRef()
    {
        Release();
    }

    void FontRef::Create(const char *font_name, int font_size)
    {
        Release();
        FontInternalCreate(this, font_name, font_size);
    }

    void Font::Render2D(void *batcher, const char *text, const Math::Vector2f &pos, int color)
    {
        FontInternalRender2D(&pos, batcher, this, text, color);
    }
}