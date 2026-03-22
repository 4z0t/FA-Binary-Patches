#include "TeamColorMode.h"
#include "Iterators.h"

int custom_colors[32]{};

enum class ETeamColorMode : std::uint8_t
{
    Default = 0,
    Alliance,
    Custom,
};

// UI_Lua TeamColorMode()
// UI_Lua TeamColorMode(1,2,3)
// UI_Lua TeamColorMode(2, {'aaa' })
// UI_Lua TeamColorMode(2, {1,2,3 })
// UI_Lua TeamColorMode(0)
// UI_Lua TeamColorMode(1)
// UI_Lua TeamColorMode(2, {'ff00ff00', 'ffffff00', 'red', 'blue' })
SHARED int TeamColorMode(lua_State *L)
{
    int top = lua_gettop(L);
    if (top < 1 || top > 2)
        L->LuaState->Error(s_ExpectedBetweenButGot, __FUNCTION__, 1, 2, top);

    if (!cwldsession)
        return 0;

    ETeamColorMode mode = (ETeamColorMode)lua_tonumber(L, 1);
    GetField<ETeamColorMode>(cwldsession, 0x4e9) = mode;

    LuaObject colors{L->LuaState, 2};
    if (!colors.IsTable())
        return 0;

    for (const auto &[i, color] : IPairs(colors))
    {
        const char *s = color.ToString();
        if (s)
        {
            int id = i - 1;
            uint32_t color_i;
            if (id >= 0 && id < 32)
            {
                if (Moho::TryConvertToColor(s, color_i))
                    custom_colors[id] = color_i;
                else
                    L->LuaState->Error(s_UnknownColor, s);
            }
        }
    }

    return 0;
}

SHARED int __thiscall SelectTeamColor(
    Moho::CWldSession *session,
    void *unit,
    struct_IconAux *aux)
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