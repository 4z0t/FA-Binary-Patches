#include "TeamColorMode.h"

int custom_colors[32]{};

SHARED int TeamColorMode(lua_State *L)
{
    LogF("TeamColorMode");
    return 0;
}

enum class ETeamColorMode : std::uint8_t
{
    Default = 0,
    Alliance,
    Custom,
};

SHARED int __thiscall SelectTeamColor(Moho::CWldSession *session, void *unit, struct_IconAux *aux)
{
    ETeamColorMode mode = GetField<ETeamColorMode>(session, 0x4e9);
    void *focus_army = session->GetFocusArmy();
    void *unit_army = GetField<void *>(unit, 0x120);
    switch (mode)
    {
    case ETeamColorMode::Custom:
    {
        int id = GetField<int>(unit_army, 0); // unit->army->id
        return custom_colors[id];
    }
    case ETeamColorMode::Alliance:
        if (focus_army != nullptr)
        {
            if (focus_army == unit_army)
                return aux->selfColor;
            EAlliance alliance = Moho__BaseArmy__GetAllianceWith(focus_army, unit_army);
            switch (alliance)
            {
            case EAlliance::ALLIANCE_Neutral:
                return aux->neutralColor;
            case EAlliance::ALLIANCE_Ally:
                return aux->allyColor;
            case EAlliance::ALLIANCE_Enemy:
                return aux->enemyColor;
            case EAlliance::ALLIANCE_None:
                return aux->neutralColor;
            }
        }
        break;
    case ETeamColorMode::Default:
        break;
    default:
        break;
    }
    return GetField<int>(unit_army, 0x148); // unit->army->playerColor
}