#pragma once
#include "FixedPool.h"

constexpr size_t TABLE_HASH_SIZE = 40;
constexpr size_t TABLE_ARRAY_SIZE = 8;
constexpr size_t TABLE_SIZE = 36;
constexpr size_t UPVALUE_SIZE = 20;
constexpr size_t PARSER_LOCAL_SIZE = 12;
constexpr size_t SMALL_SIZE = 256;

class LuaAllocator
{
public:
    LuaAllocator()
        : table_pool{},
          table_hash_pool{},
          table_array_pool{},
          upvalue_pool{},
          parser_pool{},
          small_pool{}
    {
    }

    ~LuaAllocator()
    {
    }

    void *Realloc(void *ptr, size_t old_size, size_t new_size)
    {
        if (ptr == nullptr || old_size == 0)
        {
            return Alloc(new_size);
        }

        if (new_size == 0)
        {
            Free(ptr, old_size);
            return nullptr;
        }

        LogF("Realloc: %p %d %d", ptr, old_size, new_size);

        bool is_parser = new_size % PARSER_LOCAL_SIZE == 0 && new_size <= 384;
        bool is_table = new_size == TABLE_SIZE;
        bool is_upvalue = new_size == UPVALUE_SIZE;
        bool is_table_hash = new_size % TABLE_HASH_SIZE == 0 && new_size <= 2560;
        bool is_table_array = new_size % TABLE_ARRAY_SIZE == 0 && new_size <= 1024 && !is_parser;
        bool is_small = new_size <= SMALL_SIZE;

        if (is_table_hash)
            return table_hash_pool.Realloc(ptr, old_size, new_size);
        else if (is_table_array)
            return table_array_pool.Realloc(ptr, old_size, new_size);
        else if (is_table)
            return table_pool.Realloc(ptr, old_size, new_size);
        else if (is_upvalue)
            return upvalue_pool.Realloc(ptr, old_size, new_size);
        else if (is_parser)
            return parser_pool.Realloc(ptr, old_size, new_size);
        else if (is_small)
            return small_pool.Realloc(ptr, old_size, new_size);

        void *new_ptr = Alloc(new_size);
        if (new_ptr)
        {
            memcpy(new_ptr, ptr, old_size);
            Free(ptr, old_size);
            return new_ptr;
        }
        return nullptr;
    }

    void *Alloc(size_t size)
    {
        void *ptr = InternalAlloc(size);
        LogF("Alloc: %p %d", ptr, size);
        return ptr;
    }

    void Free(void *ptr, size_t size)
    {
        LogF("Free: %p %d", ptr, size);
        bool is_parser = size % PARSER_LOCAL_SIZE == 0 && size <= 384;
        bool is_table = size == TABLE_SIZE;
        bool is_upvalue = size == UPVALUE_SIZE;
        bool is_table_hash = size % TABLE_HASH_SIZE == 0 && size <= 2560;
        bool is_table_array = size % TABLE_ARRAY_SIZE == 0 && size <= 1024 && !is_parser;
        bool is_small = size <= SMALL_SIZE;

        bool cleared = false;
        if (is_table_hash || is_upvalue) // 20
        {
            cleared = cleared ||
                      table_hash_pool.Free(ptr, size) ||
                      upvalue_pool.Free(ptr, size);
        }

        if (is_parser || is_table) // 12
        {
            cleared = cleared ||
                      table_pool.Free(ptr, size) ||
                      parser_pool.Free(ptr, size);
        }

        if (is_table_array || is_small) //  <= 128 or % 8
        {
            cleared = cleared ||
                      table_array_pool.Free(ptr, size) ||
                      small_pool.Free(ptr, size);
        }

        if (cleared)
            return;

        if (!FreeFromAll(ptr, size))
            free(ptr);
    }

private:
    void *InternalAlloc(size_t size)
    {
        bool is_parser = size % PARSER_LOCAL_SIZE == 0 && size <= 384;
        bool is_table = size == TABLE_SIZE;
        bool is_upvalue = size == UPVALUE_SIZE;
        bool is_table_hash = size % TABLE_HASH_SIZE == 0 && size <= 2560;
        bool is_table_array = size % TABLE_ARRAY_SIZE == 0 && size <= 1024 && !is_parser;
        bool is_small = size <= SMALL_SIZE;

        if (is_table_hash)
            return table_hash_pool.Alloc(size);
        else if (is_table_array)
            return table_array_pool.Alloc(size);
        else if (is_table)
            return table_pool.Alloc(size);
        else if (is_upvalue)
            return upvalue_pool.Alloc(size);
        else if (is_parser)
            return parser_pool.Alloc(size);
        else if (is_small)
            return small_pool.Alloc(size);
        else
            return malloc(size);
    }

    bool FreeFromAll(void *ptr, size_t size)
    {
        return small_pool.Free(ptr, size) ||
               table_array_pool.Free(ptr, size) ||
               table_hash_pool.Free(ptr, size) ||
               table_pool.Free(ptr, size) ||
               upvalue_pool.Free(ptr, size) ||
               parser_pool.Free(ptr, size);
    }

private:
    FixedPool<36, 64 * 1024> table_pool;
    FixedPool<80, 16 * 1024> table_hash_pool;
    FixedPool<32, 16 * 1024> table_array_pool;
    FixedPool<20, 1024> upvalue_pool;
    FixedPool<12, 1024> parser_pool;
    FixedPool<32, 1024> small_pool;
};
