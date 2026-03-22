
#include "magic_classes.h"
#include <cstdint>
#include "../UserUnit/RenderIcons.h"

enum class EAlliance : int
{
    ALLIANCE_Neutral = 0x0,
    ALLIANCE_Ally = 0x1,
    ALLIANCE_Enemy = 0x2,
    ALLIANCE_None = 0x3,
};
EAlliance __fastcall Moho__BaseArmy__GetAllianceWith(
    /*Moho::BaseArmy*/ void *a1,
    /*Moho::BaseArmy*/ void *a2) asm("0x00579430");