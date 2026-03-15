#include "Refaf/EXE/Functions.h"
#include "magic_classes.h"

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

UIRegFunc __s{
    "stat", "", [](lua_State *L) -> int
    {
        lua_pushnumber(L, Refaf::InstanceCounter<Refaf::CTask>::GetStatItem()->counter);
        return 1;
    }};
