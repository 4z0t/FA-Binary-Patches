#pragma once
#include "WeakObject.h"
#include "Utility/Pad.h"
#include "InstanceCounter.h"
#include <cstdint>

namespace Refaf
{
    class Entity
    {

        virtual ~Entity() = 0;

        Pad<0x26C> pad;
    };

    // 0x0067B5AC
    static_assert(sizeof(Entity) == 0x270);
}