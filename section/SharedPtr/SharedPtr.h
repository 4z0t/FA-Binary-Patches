#pragma once
#include "global.h"


struct SharedLock
{
    struct vtable_counted_base
    {
        void(__thiscall *dtr)(SharedLock *);
        void(__thiscall *dispose)(SharedLock *);
        void(__thiscall *destroy)(SharedLock *);
        void *(__thiscall *get_deleter)(SharedLock *);
    } *vtable;
    long use_count_;
    long weak_count_;
    void *px_;

    void InternalRelease();
    void Lock();
};

template <typename T>
class SharedPtr
{
private:
    T *data = nullptr;
    SharedLock *lock = nullptr;

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

        lock->InternalRelease();
    }

    void Lock()
    {
        if (lock)
        {
            lock->Lock();
        }
    }
};