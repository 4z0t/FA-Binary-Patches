#pragma once
#include "global.h"
#include <cstdint>

class MemoryPool
{
public:

	struct Stats
	{
		size_t size;
		size_t used_total;
		size_t total_blocks;
		size_t used_blocks;
	};

	using Byte = unsigned char;
private:
	class BlockMetaData
	{
	public:
		static const size_t META_SIZE = sizeof(size_t);
		static const size_t USED_MASK = META_SIZE - 1;
		static const size_t SIZE_MASK = ~USED_MASK;

		static const size_t BLOCK_MIN_SIZE = 64; // 64 bytes

		static size_t Align(size_t size)
		{
			return (size / META_SIZE * META_SIZE) + (size % META_SIZE != 0 ? META_SIZE : 0);
		}

		size_t GetSize()const
		{
			return data & SIZE_MASK;
		}

		bool IsUsed()const
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

		const Byte* GetDataBegin()const
		{
			return reinterpret_cast<const Byte*>(this) + sizeof(BlockMetaData);
		}

		const Byte* GetDataEnd()const
		{
			return GetDataBegin() + this->GetSize();
		}

		Byte* GetDataBegin()
		{
			return reinterpret_cast<Byte*>(this) + sizeof(BlockMetaData);
		}

		Byte* GetDataEnd()
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

		bool BelongsToChunk(void* ptr)
		{
			for (BlockMetaData* block = GetFirst(); block <= top; block = NextBlock(block))
			{
				if (block->GetDataBegin() == ptr)
					return true;
			}
			return false;
		}

		BlockMetaData* FindFirstFreeBlock(size_t size)
		{
			return nullptr;
		}

		BlockMetaData* GetBlockAt(void* ptr);


		BlockMetaData* GetFirst()
		{
			return reinterpret_cast<BlockMetaData*>(data);
		}

		BlockMetaData* NextBlock(BlockMetaData* cur)
		{
			return reinterpret_cast<BlockMetaData*>(cur->GetDataEnd());
		}

		BlockMetaData* GetTop()
		{
			return top;
		}


		void* Begin()const
		{
			return static_cast<void*>(data);
		}

		void* End()const
		{
			return static_cast<void*>(data + size);
		}

		void FreeBlock(BlockMetaData* block)
		{
			if (block->IsUsed())
			{
				used_blocks--;
				used_total -= block->GetSize() + sizeof(BlockMetaData);
				block->SetUsed(false);
				last_freed = block;
			}
		}

		void* UseBlock(BlockMetaData* block)
		{
			used_blocks++;
			used_total += block->GetSize() + sizeof(BlockMetaData);
			block->SetUsed(true);
			return block->GetDataBegin();
		}


	public:

		Chunk(size_t size);

		~Chunk();


		BlockMetaData* SplitBlock(BlockMetaData* block, size_t min_size);

		void MergeWithNext(BlockMetaData* block)
		{

		}

		void* Allocate(size_t size);

		void* Reallocate(void* ptr, size_t new_size)
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

			BlockMetaData* block = GetBlockAt(ptr);
			size_t old_size = block->GetSize();

			if (old_size < new_size)
			{
				void* new_ptr = Allocate(new_size);
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
				void* new_ptr = Allocate(new_size);
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


		void Free(void* ptr);

		bool TryFree(void* ptr);

		bool IsWithinChunk(void* ptr)const
		{
			return Begin() < ptr && ptr < End();
		}

		void SetNext(Chunk* next);

		size_t GetBlockSize(void* ptr);

		Chunk* GetNext()
		{
			return next;
		}

		const Chunk* GetNext()const
		{
			return next;
		}

		void RemoveNext()
		{
			// todo
		}

		const Byte* GetData()const
		{
			return data;
		}


		Stats GetStats()const
		{
			return {
				size,
				used_total,
				total_blocks,
				used_blocks
			};
		}


	private:
		const size_t size;
		size_t used_total;
		size_t total_blocks;
		size_t used_blocks;
		BlockMetaData* top;
		BlockMetaData* last_freed;
		Chunk* next;
		Byte* data;
	};

	Chunk* AllocateChunk(size_t chunk_size, size_t target_allocation = 0);	

public:
	MemoryPool(size_t initial_size, size_t next_size = 0) :
		next_size{ next_size != 0 ? next_size : initial_size },
		num_chunks{ 0 }
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


	template<typename T>
	T* Allocate(size_t num)
	{
		return static_cast<T*>(Allocate(num * sizeof(T)));
	}

	template<typename T>
	void Free(T* ptr)
	{
		return Free(static_cast<void*>(ptr));
	}

	Chunk* GetOwnerOfPtr(void* ptr)
	{
		Chunk* cur = head;

		while (cur != nullptr)
		{
			if (cur->IsWithinChunk(ptr))
				return cur;

			cur = cur->GetNext();
		}
		return nullptr;
	}

	void* Reallocate(void* ptr, size_t new_size);

	void* Allocate(size_t size)
	{
		if (size == 0)
			return nullptr;

		Chunk* cur = head;
		Chunk* prev = nullptr;

		void* data = nullptr;
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

		Chunk* new_chunk = AllocateChunk(next_size, size);
		if (new_chunk == nullptr)
		{
			return nullptr; // couldn't allocate that many
		}

		num_chunks++;
		prev->SetNext(new_chunk);
		return new_chunk->Allocate(size);
	}

	void Free(void* ptr)
	{
		if (ptr == nullptr)
			return;

		Chunk* cur = head;
		while (cur != nullptr)
		{
			if (cur->TryFree(ptr))
				break;
			cur = cur->GetNext();
		}

		// check if more than 2 chunks are empty and remove one of them
	}

	Stats GetStats(size_t index) const
	{
		const Chunk* cur = head;
		size_t i = 0;
		while (cur != nullptr)
		{
			if (i == index)
				break;
			cur = cur->GetNext();
		}
		if (cur)
		{
			return cur->GetStats();
		}
		return {};
	}

	private:
	size_t next_size;
	size_t num_chunks;
	Chunk* head;
};