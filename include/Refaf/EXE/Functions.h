#pragma once
#include "Refaf/UI/Font.h"
#include "Refaf/InstanceCounter.h"
#include "Refaf/UserUnit.h"
#include "Refaf/Entity.h"
#include "Refaf/CTask.h"

namespace Refaf
{
    template <>
    StatItem *InstanceCounter<CTask>::GetStatItem() asm("0x0040AB50");

    void __stdcall Font__Dtor(Font *font) asm("0x425860");

    SHARED
    {
        void *InternalCreate(FontRef * font, const char *font_name, int font_size);
    }
} // namespace Refaf