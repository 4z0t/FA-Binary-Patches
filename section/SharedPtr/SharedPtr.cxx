#include "SharedPtr.h"

void SharedLock::InternalRelease()
{
    if (_InterlockedExchangeAdd(&use_count_, -1))
        return;

    vtable->dispose(this);
    if (_InterlockedExchangeAdd(&weak_count_, -1))
        return;

    vtable->destroy(this);
}

void SharedLock::Lock()
{
    _InterlockedExchangeAdd(&use_count_, 1);
}