#pragma once
#include "global.h"

template <typename T>
struct SharedPtr
{
    T *data = nullptr;

    struct SharedLock
    {
        struct vtable_counted_base
        {
            void(__thiscall *dtr)(SharedLock *);
            void(__thiscall *dispose)(SharedLock *);
            void(__thiscall *destroy)(SharedLock *);
            void *(__thiscall *get_deleter)(SharedLock *);
        } *vtable;
        unsigned use_count_;
        unsigned weak_count_;
        void *px_;
    } *lock = nullptr;

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

    void Lock()
    {
        if (lock)
        {
            InterlockedExchangeAdd(&lock->use_count_, 1);
        }
    }

    void Release()
    {
        if (!lock)
            return;

        if (InterlockedExchangeAdd(&lock->use_count_, -1))
            return;

        lock->vtable->dispose(lock);
        if (InterlockedExchangeAdd(&lock->weak_count_, -1))
            return;

        lock->vtable->destroy(lock);
    }

    ~SharedPtr()
    {
        Release();
        lock = nullptr;
        data = nullptr;
    }
};