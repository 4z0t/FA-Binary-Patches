#include "Refaf/InstanceCounter.h"

namespace Refaf
{
    void InstanceCounterBase::Increment(long volatile *counter)
    {
        _InterlockedExchangeAdd(counter, 1);
    }
    void InstanceCounterBase::Decrement(long volatile *counter)
    {
        _InterlockedExchangeAdd(counter, -1);
    }
}
