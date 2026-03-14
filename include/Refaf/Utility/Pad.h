#pragma once
#include <cstdint>
#include <cstddef>

namespace Refaf
{
    template <size_t N>
    class Pad
    {
        std::byte _pad[N];
    };
} // namespace Refaf
