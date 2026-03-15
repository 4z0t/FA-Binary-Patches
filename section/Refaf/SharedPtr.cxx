
#include "Refaf/SharedPtr.h"

namespace Refaf
{
    void CountedLock::Release()
    {
        if (_InterlockedExchangeAdd(&this->use_count_, -1))
            return;

        this->Dispose();
        if (_InterlockedExchangeAdd(&this->weak_count_, -1))
            return;

        this->Destroy();
    }

    void CountedLock::Lock()
    {
        _InterlockedExchangeAdd(&this->use_count_, 1);
    }
}