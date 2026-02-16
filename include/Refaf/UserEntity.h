#pragma once
#include "WeakObject.h"
#include "Utility/Pad.h"
#include "CWldSession.h"
#include <cstdint>

namespace Refaf
{
    class UserUnit;
    class UserEntity : public Pad<4> /* dead value */, public WeakObject<UserEntity>
    {
    public:
        CWldSession *session;
        Pad<312> pad;

        virtual ~UserEntity() {}

        virtual void Tick() {}
        virtual UserUnit *IsUserUnit() { return nullptr; }
        virtual UserUnit *ToUserUnit() { return nullptr; }
        virtual float GetUniformScale() { return 1.0f; }
    };

    // 0x0089495D
    static_assert(sizeof(UserEntity) == 328);
}