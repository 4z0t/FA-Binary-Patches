#pragma once
#include "Refaf/UI/Font.h"
#include "Refaf/Console/ConCommand.h"
#include "Refaf/Console/TConVar.h"
#include "Refaf/InstanceCounter.h"
#include "Refaf/UserUnit.h"
#include "Refaf/Entity.h"
#include "Refaf/CTask.h"

namespace Refaf
{
    template <>
    StatItem *InstanceCounter<CTask>::GetStatItem() asm("0x0040AB50");

    void __thiscall Con_RegisterConCommand(ConCommandMap *map, const ConCommand *cmd) asm("0x41E390");

    ConCommandMap *Con_GetMap() asm("0x41BEB0");

    void __stdcall Font__Dtor(Font *font) asm("0x425860");

    SHARED
    {
        void Con_UnregisterCommand(ConCommandMap * map, const ConCommand *command);
        void *FontInternalCreate(FontRef * font, const char *font_name, int font_size);
        void *FontInternalRender2D(const Math::Vector2f *pos, void *batcher, Font *font, const char *chText, int color);
    }
} // namespace Refaf