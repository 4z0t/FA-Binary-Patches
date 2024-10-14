#pragma once
#include "moho.h"

template <typename T>
struct BaseVector
{

    struct _Iterator
    {
        _Iterator(T *it) : it(it) {}

        bool operator==(const _Iterator &other) { return other.it == it; };
        bool operator!=(const _Iterator &other) { return !(*this == other); };

        _Iterator &operator++()
        {
            ++it;
            return *this;
        }

        T &operator*() { return *it; }

    private:
        T *it;
    };

    using Iterator = _Iterator;

    T &operator[](int index) { return this->_begin[index]; }

    Iterator begin() { return Iterator(this->_begin); }
    Iterator end() { return Iterator(this->_end); }

public:
    T *_begin;
    T *_end;
    T *_capacity_end;
};

template <typename T>
struct BaseSelf
{
    T *self;
};

template <typename T, size_t N>
struct InlinedVector : BaseVector<T>
{
    T *inlined;
    T inlined_items[N];

    InlinedVector()
    {
        this->_begin = inlined_items;
        this->_end = inlined_items;
        this->_capacity_end = inlined_items + N;
        inlined = inlined_items;
    }

    ~InlinedVector()
    {
        if (this->_begin != inlined)
        {
            free(this->_begin);
            this->_begin = inlined_items;
            this->_end = inlined_items;
            this->_capacity_end = inlined_items + N;
        }
    }
};
template <typename T>
struct FastVector : BaseSelf<FastVector<T>>, BaseVector<T>
{
    size_t Size() const
    {
        return this->_begin ? this->_end - this->_begin : 0;
    }

    size_t Capacity() const
    {
        return this->_begin ? this->_capacity_end - this->_begin : 0;
    }
};