#pragma once

namespace Refaf
{
    class IUnit
    {
        virtual bool IsUnit() = 0;
    };

    static_assert(sizeof(IUnit) == 4);
}