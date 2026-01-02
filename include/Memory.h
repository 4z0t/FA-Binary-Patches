#pragma once
#include <cstdint>

namespace Moho
{
    struct nothrow_t
    {
        explicit nothrow_t() = default;
    };

    extern const nothrow_t nothrow;
}
