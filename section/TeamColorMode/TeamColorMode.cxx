#include "TeamColorMode.h"

SHARED int TeamColorMode(lua_State *L)
{
    LogF("TeamColorMode");
    return 0;
}

enum class ETeamColorMode : std::uint8_t
{
    Default = 0,
    Alliance,

};

SHARED int __thiscall SelectTeamColor(void *session, void *unit, void *aux)
{
    ETeamColorMode mode = GetField<ETeamColorMode>(session, 0x4e9);
    switch (mode)
    {
    case ETeamColorMode::Alliance:
        
        return 0;
    case ETeamColorMode::Default:
    default:
        return GetField<int>(GetField<void *>(unit, 0x120), 0x148); // unit->army->playerColor
    }
}