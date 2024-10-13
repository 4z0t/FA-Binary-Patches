#pragma once
#include "global.h"

template <typename T>
class Vector
{
private:
    T *_begin;
    T *_end;
    T *_capacity_end;

public:
    Vector() : _begin{nullptr},
               _end{nullptr},
               _capacity_end{nullptr}
    {
    }

    Vector(size_t n) : _begin{new T[n]{}}, _end{_begin}, _capacity_end{_begin}
    {
    }

    size_t Size() const
    {
        return _begin != nullptr ? _end - _begin : 0;
    }

    T &operator[](int index)
    {
        return _begin[index];
    }

    const T &operator[](int index) const
    {
        return _begin[index];
    }

    ~Vector()
    {
        if (_begin)
            delete[] _begin;
        _begin = nullptr;
        _end = nullptr;
        _capacity_end = nullptr;
    }
};
