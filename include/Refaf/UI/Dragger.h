#pragma once
#include "Refaf/WeakObject.h"
#include "Refaf/Utility/Pad.h"
#include "EventData.h"
#include <cstdint>

namespace Refaf
{
    class IDragger : public WeakObject<IDragger>
    {
    public:
        // 0x0078DB90
        virtual ~IDragger() = default;

        virtual void OnMove(EventData *event)
        {
        }

        virtual void OnRelease(EventData *event)
        {
            delete this;
        }

        virtual void OnCancel()
        {
            delete this;
        }
    };

    static_assert(sizeof(IDragger) == 8);
}