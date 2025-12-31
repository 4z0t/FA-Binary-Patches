#include "global.h"
#include "Vector.h"
#include <new>

void *operator new(std::size_t n, const std::nothrow_t &tag) noexcept
{
    return malloc(n);
}

void *operator new[](std::size_t n, const std::nothrow_t &tag) noexcept
{
    return malloc(n);
}

void *operator new(std::size_t n) noexcept(false)
{
    return shi_new(n);
}
void operator delete(void *p) throw()
{
    free(p);
}

void *operator new[](std::size_t n) noexcept(false)
{
    return shi_new(n);
}
void operator delete[](void *p) throw()
{
    free(p);
}

void operator delete[](void *p, unsigned int) throw()
{
    free(p);
}
