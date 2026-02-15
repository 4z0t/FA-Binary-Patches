#pragma once
#include <cstdint>

namespace Refaf
{
    template <size_t N>
    struct Pad
    {
        std::byte _pad[N];
    };
} // namespace Refaf
