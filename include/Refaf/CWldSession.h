#pragma once
#include "Utility/Pad.h"

namespace Refaf
{
    class CWldSession
    {
        Pad<1288> pad;
    };

    // 0x00897238
    static_assert(sizeof(CWldSession) == 1288);

}