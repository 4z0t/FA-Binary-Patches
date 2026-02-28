#include "InstanceCounterGetStatItem.h"
#include "magic_classes.h"

UIRegFunc __s{
    "stat", "", [](lua_State *L) -> int
    {
        lua_pushnumber(L, Refaf::InstanceCounter<Refaf::CTask>::GetStatItem()->counter);
        return 1;
    }};
