#pragma once
#include "Refaf/Math/Vector2.h"

namespace Refaf
{
    struct EventData
    {
        int type;
        Math::Vector2f mouse_pos;

        int wheel_rotation;
        int wheel_data;
        int key_code;
        int raw_key_code;
        int modifiers;
        void *source;
    };

    static_assert(sizeof(EventData) == 0x24);
}
