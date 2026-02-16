#pragma once
#include "UserEntity.h"
#include "IUnit.h"

namespace Refaf
{
    class UserUnit : UserEntity, IUnit, public WeakObject<UserUnit>
    {
        Pad<664> pad2;
    };

    // 0x0089495D
    static_assert(sizeof(UserUnit) == 1000);
}