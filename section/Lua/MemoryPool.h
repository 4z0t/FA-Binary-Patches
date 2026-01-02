#pragma once

#include <cstdint>
#include "global.h"

#ifdef _DEBUG
#define assert(expr)                        \
    do                                      \
    {                                       \
        if (!(expr))                        \
        {                                   \
            WarningF("%s", #expr);          \
            CRASH_GAME(); \
        }                                   \
    } while (false)
#else
#define assert(expr)
#endif
class MemoryPool
{
    using Byte = unsigned char;

    class BlockMetaData
    {
    public:
        static const size_t META_SIZE = sizeof(size_t);
        static const size_t USED_MASK = META_SIZE - 1;
        static const size_t SIZE_MASK = ~USED_MASK;

        static const size_t BLOCK_MIN_SIZE = 64; // 64 bytes

        static size_t Allign(size_t size)
        {
            return (size / META_SIZE * META_SIZE) + (size % META_SIZE != 0 ? META_SIZE : 0);
        }

        size_t GetSize() const
        {
            return data & SIZE_MASK;
        }

        bool IsUsed() const
        {
            return static_cast<bool>(data & USED_MASK);
        }

        void SetUsed(bool used)
        {
            Set(GetSize(), used);
        }

        void SetSize(size_t size)
        {
            Set(size, IsUsed());
        }

        const Byte *GetDataBegin() const
        {
            return reinterpret_cast<const Byte *>(this) + sizeof(BlockMetaData);
        }

        const Byte *GetDataEnd() const
        {
            return GetDataBegin() + this->GetSize();
        }

        Byte *GetDataBegin()
        {
            return reinterpret_cast<Byte *>(this) + sizeof(BlockMetaData);
        }

        Byte *GetDataEnd()
        {
            return GetDataBegin() + this->GetSize();
        }

    private:
        void Set(size_t size, bool used)
        {
            data = (size & SIZE_MASK) | (static_cast<size_t>(used) & USED_MASK);
        }

        size_t data;
    };

    class Chunk
    {
        static_assert(sizeof(Byte) == 1, "Byte must be exactly 1 byte");

        class Iterator
        {
            Iterator()
            {
            }
        };

        bool BelongsToChunk(void *ptr)
        {
            for (BlockMetaData *block = GetFirst(); block <= top; block = NextBlock(block))
            {
                if (block->GetDataBegin() == ptr)
                    return true;
            }
            return false;
        }

        BlockMetaData *FindFirstFreeBlock(size_t size)
        {
            return nullptr;
        }

        BlockMetaData *GetBlockAt(void *ptr)
        {
            assert(BelongsToChunk(ptr));
            return reinterpret_cast<BlockMetaData *>(ptr) - 1;
        }

        BlockMetaData *GetFirst()
        {
            return reinterpret_cast<BlockMetaData *>(data);
        }

        BlockMetaData *NextBlock(BlockMetaData *cur)
        {
            return reinterpret_cast<BlockMetaData *>(cur->GetDataEnd());
        }

        BlockMetaData *GetTop()
        {
            return top;
        }

        void *Begin() const
        {
            return static_cast<void *>(data);
        }

        void *End() const
        {
            return static_cast<void *>(data + size);
        }

    public:
        Chunk(size_t size) : size{BlockMetaData::Allign(size)},
                             used_total{0},
                             used_active{0},
                             total_blocks{0},
                             used_blocks{0},
                             top{nullptr},
                             next{nullptr},
                             data{new (Moho::nothrow) Byte[this->size]}
        {
            if (data) // in case we have failed to allocate data for chunk
            {
                top = static_cast<BlockMetaData *>(Begin());
                BlockMetaData *block = top;
                block->SetSize(this->size - sizeof(BlockMetaData));
                block->SetUsed(false);
                used_total += sizeof(BlockMetaData);
                used_active += sizeof(BlockMetaData);
                total_blocks++;
            }
        }

        ~Chunk()
        {
            delete next;
            next = nullptr;

            delete[] data;
            data = nullptr;

            top = nullptr;
            used_total = 0;
            used_active = 0;
            total_blocks = 0;
            used_blocks = 0;
        }

        BlockMetaData *SplitBlock(BlockMetaData *block, size_t min_size)
        {
            size_t prev_size = block->GetSize();
            BlockMetaData *new_block = reinterpret_cast<BlockMetaData *>(block->GetDataBegin() + min_size);
            block->SetSize(min_size);
            new_block->SetSize(prev_size - min_size - sizeof(BlockMetaData));
            new_block->SetUsed(false);
            return new_block;
        }

        void MergeWithNext(BlockMetaData *block)
        {
        }

        void *Allocate(size_t size)
        {
            if (size == 0)
                return nullptr;
            size = BlockMetaData::Allign(size);

            if (size_t top_size = top->GetSize(); !top->IsUsed() && top_size >= size)
            {
                size_t diff = top_size - size;
                if (diff < BlockMetaData::BLOCK_MIN_SIZE * 2)
                {
                    top->SetUsed(true);
                    return top->GetDataBegin();
                }
                else if (top_size >= BlockMetaData::BLOCK_MIN_SIZE * 4) // top is too big, split it
                {
                    BlockMetaData *new_block = SplitBlock(top, std::max(size, BlockMetaData::BLOCK_MIN_SIZE));
                    BlockMetaData *prev_top = top;
                    top = new_block;

                    prev_top->SetUsed(true);
                    return prev_top->GetDataBegin();
                }
                else
                {
                    top->SetUsed(true);
                    return top->GetDataBegin();
                }
            }

            for (BlockMetaData *block = GetFirst(); block <= top; block = NextBlock(block))
            {
            }
            return nullptr;
        }

        void *Reallocate(void *ptr, size_t new_size)
        {
            if (ptr == nullptr)
            {
                return Allocate(new_size);
            }

            if (new_size == 0)
            {
                Free(ptr);
                return nullptr;
            }

            BlockMetaData *block = GetBlockAt(ptr);
            size_t old_size = block->GetSize();

            if (old_size < new_size)
            {
                void *new_ptr = Allocate(new_size);
                if (new_ptr)
                {
                    memcpy(new_ptr, ptr, old_size);
                    Free(ptr);
                    return new_ptr;
                }
                return nullptr;
            }

            if (old_size / 2 >= new_size)
            {
                void *new_ptr = Allocate(new_size);
                if (new_ptr)
                {
                    memcpy(new_ptr, ptr, new_size);
                    Free(ptr);
                    return new_ptr;
                }
                return nullptr;
            }

            return ptr;
        }

        void Free(void *ptr)
        {
            if (ptr)
                GetBlockAt(ptr)->SetUsed(false);
        }

        bool TryFree(void *ptr)
        {
            if (IsWithinChunk(ptr))
            {
                assert(BelongsToChunk(ptr));
                Free(ptr);
                return true;
            }
            return false;
        }

        bool IsWithinChunk(void *ptr) const
        {
            return Begin() < ptr && ptr < End();
        }

        void SetNext(Chunk *next)
        {
            assert(this->next == nullptr);
            this->next = next;
        }

        size_t GetBlockSize(void *ptr)
        {
            assert(BelongsToChunk(ptr));
            return GetBlockAt(ptr)->GetSize();
        }

        Chunk *GetNext()
        {
            return next;
        }

        void RemoveNext()
        {
            // todo
        }

        const Byte *GetData() const
        {
            return data;
        }

    private:
        const size_t size;
        size_t used_total;
        size_t used_active;
        size_t total_blocks;
        size_t used_blocks;
        BlockMetaData *top;
        Chunk *next;
        Byte *data;
    };

    Chunk *AllocateChunk(size_t chunk_size, size_t target_allocation = 0)
    {
        size_t size = chunk_size;
        if (target_allocation * 2 >= chunk_size) // very large block to allocate, have to make chunk of bigger size than we expected
        {
            size = (target_allocation / chunk_size + 2) * chunk_size; // Keep it in multiples of chunk size
        }

        Chunk *chunk = new (Moho::nothrow) Chunk(size);
        if (chunk == nullptr || chunk->GetData() == nullptr) // couldn't allocate chunk
        {
            // here we free it back
            delete chunk;
            return nullptr;
        }
        return chunk;
    }

public:
    MemoryPool(size_t initial_size, size_t next_size = 0) : next_size{next_size != 0 ? next_size : initial_size},
                                                            num_chunks{0}
    {
        head = AllocateChunk(initial_size);
        if (head)
        {
            num_chunks = 1;
        }
    }

    ~MemoryPool()
    {
        delete head;
        head = nullptr;
    }

    template <typename T>
    T *Allocate(size_t num)
    {
        return static_cast<T *>(Allocate(num * sizeof(T)));
    }

    template <typename T>
    void Free(T *ptr)
    {
        return Free(static_cast<void *>(ptr));
    }

    Chunk *GetOwnerOfPtr(void *ptr)
    {
        Chunk *cur = head;

        while (cur != nullptr)
        {
            if (cur->IsWithinChunk(ptr))
                return cur;

            cur = cur->GetNext();
        }
        return nullptr;
    }

    void *Reallocate(void *ptr, size_t new_size)
    {
        if (ptr == nullptr)
        {
            return Allocate(new_size);
        }

        if (new_size == 0)
        {
            Free(ptr);
            return nullptr;
        }

        Chunk *owner = GetOwnerOfPtr(ptr);
        void *data = owner->Reallocate(ptr, new_size);
        if (data != nullptr)
            return data;

        size_t old_size = owner->GetBlockSize(ptr);

        if (old_size < new_size)
        {
            void *new_ptr = Allocate(new_size);
            if (new_ptr)
            {
                memcpy(new_ptr, ptr, old_size);
                owner->Free(ptr);
                return new_ptr;
            }
            return nullptr;
        }

        if (old_size / 2 >= new_size)
        {
            void *new_ptr = Allocate(new_size);
            if (new_ptr)
            {
                memcpy(new_ptr, ptr, new_size);
                owner->Free(ptr);
                return new_ptr;
            }
            return nullptr;
        }

        // unreachable?
        assert(false);
        return ptr;
    }

    void *Allocate(size_t size)
    {
        if (size == 0)
            return nullptr;

        Chunk *cur = head;
        Chunk *prev = nullptr;

        void *data = nullptr;
        while (cur != nullptr)
        {
            data = cur->Allocate(size);
            if (data != nullptr)
                return data;

            prev = cur;
            cur = cur->GetNext();
        }

        if (prev == nullptr) // no head, something went wrong.
        {
            return nullptr;
        }

        Chunk *new_chunk = AllocateChunk(next_size, size);
        if (new_chunk == nullptr)
        {
            return nullptr; // couldn't allocate that many
        }

        num_chunks++;
        prev->SetNext(new_chunk);
        return new_chunk->Allocate(size);
    }

    void Free(void *ptr)
    {
        if (ptr == nullptr)
            return;

        Chunk *cur = head;
        while (cur != nullptr)
        {
            if (cur->TryFree(ptr))
                break;
            cur = cur->GetNext();
        }

        // check if more than 2 chunks are empty and remove one of them
    }

private:
    size_t next_size;
    size_t num_chunks;
    Chunk *head;
};
