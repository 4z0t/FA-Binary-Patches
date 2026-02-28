#pragma once
#include "Utility/Pad.h"
#include "InstanceCounter.h"
#include <cstdint>

namespace Refaf
{

    class CTaskThread
    {
    };

    class CTask : InstanceCounter<CTask>
    {
        virtual ~CTask() = 0;

        Pad<0x14> pad;
    };

    // 0x0067B5AC
    static_assert(sizeof(CTask) == 0x18);
}