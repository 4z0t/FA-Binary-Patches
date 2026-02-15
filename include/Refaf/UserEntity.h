#pragma once
#include "WeakObject.h"
#include "Utility/Pad.h"
#include "CWldSession.h"
#include <cstdint>

namespace Refaf
{
    class UserEntity : Pad<8> /* vtable and dead value */, public WeakObject<UserEntity>
    {
    public:
        CWldSession *session;
        Pad<312> pad;
    };

    // 0x0089495D
    static_assert(sizeof(UserEntity) == 328);
}