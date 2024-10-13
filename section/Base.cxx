#include "global.h"
#include "Vector.h"

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
