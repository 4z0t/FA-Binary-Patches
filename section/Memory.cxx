#include "Memory.h"

void *operator new(std::size_t n, const Moho::nothrow_t &tag) noexcept
{
    return malloc(n);
}

void *operator new[](std::size_t n, const Moho::nothrow_t &tag) noexcept
{
    return malloc(n);
}

void *operator new(std::size_t n) noexcept(false)
{
    return shi_new(n);
}
void *operator new[](std::size_t n) noexcept(false)
{
    return shi_new(n);
}
void operator delete(void *p) noexcept
{
    free(p);
}
void operator delete[](void *p) noexcept
{
    free(p);
}
void operator delete(void *p, unsigned int) noexcept
{
    free(p);
}
void operator delete[](void *p, unsigned int) noexcept
{
    free(p);
}

const Moho::nothrow_t Moho::nothrow{};