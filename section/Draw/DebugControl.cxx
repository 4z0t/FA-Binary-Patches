#include "DebugControl.h"
#include "Refaf/UI/Font.h"

static Refaf::FontRef default_font;

void __thiscall _DrawDebugControl(void *control, void *batcher, Vector3f *tl)
{
    if (default_font.Get() == nullptr)
    {
        default_font.Create("Arial", 12);
    }

    string *name = Offset<string *>(control, 0x100);

    Refaf::Math::Vector2f pos = {tl->x, tl->y - 5.f};
    default_font.Get()->Render2D(batcher, name->data(), pos, 0xFFFF00FF);
}
