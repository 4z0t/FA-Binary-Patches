#pragma once
#include "Utility/Pad.h"

namespace Refaf
{
    class CountedLock
    {
        long use_count_;
        long weak_count_;
        void *px_;

    public:
        virtual ~CountedLock() = 0;
        virtual void Dispose() = 0;
        virtual void Destroy() = 0;
        virtual void *GetDeleter() = 0;

        void Release();

        void Lock();
    };

    template <typename T>
    class SharedPtr
    {
        T *data;
        CountedLock *lock;

    public:
        SharedPtr() : data{nullptr}, lock{nullptr}
        {
        }

        SharedPtr(const SharedPtr &other)
        {
            data = other.data;
            lock = other.lock;
            Lock();
        }

        SharedPtr(SharedPtr &&other)
        {
            data = other.data;
            lock = other.lock;
            other.data = nullptr;
            other.lock = nullptr;
        }

        SharedPtr &operator=(const SharedPtr<T> &other)
        {
            Release();
            data = other.data;
            lock = other.lock;
            Lock();
        }

        SharedPtr &operator=(SharedPtr &&other)
        {
            Release();
            data = other.data;
            lock = other.lock;
            other.data = nullptr;
            other.lock = nullptr;
        }

        T *Get() const
        {
            return data;
        }

        bool Valid() const
        {
            return data != nullptr;
        }

        ~SharedPtr()
        {
            Release();
        }

        void Release()
        {
            InternalRelease();
            lock = nullptr;
            data = nullptr;
        }

    private:
        void InternalRelease()
        {
            if (!lock)
                return;

            lock->Release();
        }

        void Lock()
        {
            if (lock)
            {
                lock->Lock();
            }
        }
    };

    static_assert(sizeof(SharedPtr<void>) == 8);
}