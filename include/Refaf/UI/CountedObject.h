#pragma once
#include "Refaf/Utility/Base.h"

namespace Refaf
{
    class CountedObject
    {
    public:
        int refcount;

        CountedObject() : refcount{1} {}

        void Release()
        {
            if (--refcount == 0)
                delete this;
        }

        virtual ~CountedObject() {}
    };
}