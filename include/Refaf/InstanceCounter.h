#pragma once
#include "Utility/Pad.h"

namespace Refaf
{

    class StatItem
    {
        virtual ~StatItem() = 0;

    public:
        Pad<0x20> pad1;
        long counter;
        Pad<0x78> pad2;
    };

    // 0x0040D208
    static_assert(sizeof(StatItem) == 0xA0);

    template <typename T>
    class InstanceCounter
    {
    public:
        InstanceCounter() { _InterlockedExchangeAdd(&GetStatItem()->counter, 1); }
        ~InstanceCounter() { _InterlockedExchangeAdd(&GetStatItem()->counter, -1); }

        static StatItem *GetStatItem();
    };
} // namespace Refaf
