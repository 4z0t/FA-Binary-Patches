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

    class InstanceCounterBase
    {
    protected:
        static void Increment(long volatile *counter);
        static void Decrement(long volatile *counter);
    };

    template <typename T>
    class InstanceCounter : InstanceCounterBase
    {
    public:
        InstanceCounter() { Increment(&GetStatItem()->counter); }
        ~InstanceCounter() { Decrement(&GetStatItem()->counter); }

        static StatItem *GetStatItem();
    };
} // namespace Refaf
