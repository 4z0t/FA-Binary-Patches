#pragma once
#define SHARED extern "C"

template <typename T = void *>
T Offset(void *ptr, size_t offset)
{
    return reinterpret_cast<T>(reinterpret_cast<char *>(ptr) + offset);
}

template <typename T = void *>
const T Offset(const void *ptr, size_t offset)
{
    return reinterpret_cast<const T>(reinterpret_cast<const char *>(ptr) + offset);
}

template <typename T = void *>
T &GetField(void *ptr, size_t offset)
{
    return *Offset<T *>(ptr, offset);
}

template <typename T = void *>
const T &GetField(const void *ptr, size_t offset)
{
    return *Offset<T *>(ptr, offset);
}