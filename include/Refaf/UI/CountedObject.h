#pragma once
#include "Refaf/Utility/Base.h"

namespace Refaf
{
    class CountedObject
    {
        int refcount;

    public:
        // 0x004228D0
        CountedObject()
            : refcount{0}
        {
        }

        // 0x00422910
        void Release()
        {
            if (--refcount == 0)
                delete this;
        }

        // 0x00422900
        void Acquire()
        {
            ++refcount;
        }

        // 0x004228C0
        virtual ~CountedObject()
        {
        }
    };
}