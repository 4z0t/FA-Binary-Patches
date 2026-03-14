#include "DebugControl.h"

void __thiscall _DrawDebugControl(void *control, void *batcher, Vector3f *tl)
{
    if (default_font == nullptr)
        return;

    string *name = Offset<string *>(control, 0x100);

    Vector2f pos = {tl->x, tl->y - 5.f};
    Moho__CD3DFont__Render2D(&pos, batcher, default_font, name->data(), 0xFFFF00FF);
}
