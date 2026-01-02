#include "MemoryPool.h"


MemoryPool::BlockMetaData* MemoryPool::Chunk::GetBlockAt(void* ptr)
{
	assert(BelongsToChunk(ptr));
	return reinterpret_cast<BlockMetaData*>(ptr) - 1;
}

MemoryPool::Chunk::Chunk(size_t size) :
	size{ BlockMetaData::Align(size) },
	used_total{ 0 },
	total_blocks{ 0 },
	used_blocks{ 0 },
	top{ nullptr },
	last_freed{ nullptr },
	next{ nullptr },
	data{ new(std::nothrow) Byte[this->size] }
{
	if (data) // in case we have failed to allocate data for chunk
	{
		top = static_cast<BlockMetaData*>(Begin());
		BlockMetaData* block = top;
		block->SetSize(this->size - sizeof(BlockMetaData));
		block->SetUsed(false);
		total_blocks++;
	}
}

MemoryPool::Chunk::~Chunk()
{
	delete next;
	next = nullptr;

	delete[] data;
	data = nullptr;

	last_freed = nullptr;

	top = nullptr;
	used_total = 0;
	total_blocks = 0;
	used_blocks = 0;
}


bool MemoryPool::Chunk::TryFree(void* ptr)
{
	if (IsWithinChunk(ptr))
	{
		assert(BelongsToChunk(ptr));
		Free(ptr);
		return true;
	}
	return false;
}

void MemoryPool::Chunk::SetNext(Chunk* next)
{
	assert(this->next == nullptr);
	this->next = next;
}

size_t MemoryPool::Chunk::GetBlockSize(void* ptr)
{
	assert(BelongsToChunk(ptr));
	return GetBlockAt(ptr)->GetSize();
}

MemoryPool::BlockMetaData* MemoryPool::Chunk::SplitBlock(BlockMetaData* block, size_t min_size)
{
	size_t prev_size = block->GetSize();
	BlockMetaData* new_block = reinterpret_cast<BlockMetaData*>(block->GetDataBegin() + min_size);
	block->SetSize(min_size);
	new_block->SetSize(prev_size - min_size - sizeof(BlockMetaData));
	new_block->SetUsed(false);
	total_blocks++;
	return new_block;
}


void* MemoryPool::Chunk::Allocate(size_t size)
{
	if (size == 0) return nullptr;
	size = BlockMetaData::Align(size);

	if (size - used_total < size || used_blocks == total_blocks)
		return nullptr;

	size_t top_size = top->GetSize();
	if (!top->IsUsed() && top_size >= size)
	{
		size_t diff = top_size - size;
		if (diff < BlockMetaData::BLOCK_MIN_SIZE * 2)
		{
			return UseBlock(top);
		}
		else if (top_size >= BlockMetaData::BLOCK_MIN_SIZE * 4) // top is too big, split it
		{
			BlockMetaData* new_block = SplitBlock(top, std::max(size, BlockMetaData::BLOCK_MIN_SIZE));
			BlockMetaData* prev_top = top;
			top = new_block;

			return UseBlock(prev_top);
		}
		else
		{
			return UseBlock(top);
		}
	}

	if (last_freed)
	{
		if (last_freed->IsUsed())
		{
			last_freed = nullptr;
		}
		else if (last_freed->GetSize() >= size)
		{
			last_freed = nullptr;
			return UseBlock(last_freed);
		}
	}

	for (BlockMetaData* block = GetFirst(); block <= top; block = NextBlock(block))
	{
		if (!block->IsUsed() && block->GetSize() >= size)
		{
			return UseBlock(block);
		}
	}
	return nullptr;

}

void MemoryPool::Chunk::Free(void* ptr)
{
	if (ptr)
	{
		FreeBlock(GetBlockAt(ptr));
	}
}

MemoryPool::Chunk* MemoryPool::AllocateChunk(size_t chunk_size, size_t target_allocation)
{
	size_t size = chunk_size;
	if (target_allocation * 2 >= chunk_size) // very large block to allocate, have to make chunk of bigger size than we expected
	{
		size = (target_allocation / chunk_size + 2) * chunk_size; // Keep it in multiples of chunk size
	}

	Chunk* chunk = new(std::nothrow) Chunk(size);
	if (chunk == nullptr || chunk->GetData() == nullptr) // couldn't allocate chunk
	{
		// here we free it back
		delete chunk;
		return nullptr;
	}
	return chunk;
}

void* MemoryPool::Reallocate(void* ptr, size_t new_size)
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

	Chunk* owner = GetOwnerOfPtr(ptr);
	void* data = owner->Reallocate(ptr, new_size);
	if (data != nullptr)
		return data;

	size_t old_size = owner->GetBlockSize(ptr);

	if (old_size < new_size)
	{
		void* new_ptr = Allocate(new_size);
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
		void* new_ptr = Allocate(new_size);
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