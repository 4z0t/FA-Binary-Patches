#include "Allocator.h"
#include "MemoryPool.h"

void *__cdecl MP_ReallocFunction(
    void *ptr,
    unsigned int oldsize,
    unsigned int size,
    void *data,
    const char *allocName,
    unsigned int flags)
{
    // LogF("%u->%u", oldsize, size);
    MemoryPool *pool = (MemoryPool *)data;

    return pool->Reallocate(ptr, size);
}

void __cdecl MP_FreeFunction(void *ptr, unsigned int oldsize, void *data)
{
    MemoryPool *pool = (MemoryPool *)data;
    pool->Free(ptr);
}

size_t distrib[512]{};

/*
40, 80, 160, 320 - table
not mult of 4/8 - string
8 - userdata
32 - function
*/

void *__cdecl my_ReallocFunction(
    void *ptr,
    unsigned int oldsize,
    unsigned int size,
    void *data,
    const char *allocName,
    unsigned int flags)
{
    if (size < 512)
    {
        distrib[size]++;
    }
    return realloc(ptr, size);
}

void __cdecl my_FreeFunction(void *ptr, unsigned int oldsize, void *data)
{
    free(ptr);
}

SHARED LuaState *__thiscall UI_StateCreate(LuaState *_this, StandardLibraries libs)
{
    LogF("UI_StateCreate: %p", _this);

    MemoryPool *pool = new (std::nothrow) MemoryPool(0x4000000, 0x1000000);

    // lua_setdefaultmemoryfunctions(my_ReallocFunction, my_FreeFunction, pool);
    lua_setdefaultmemoryfunctions(MP_ReallocFunction, MP_FreeFunction, pool);

    new (_this) LuaState(libs);

    lua_setdefaultmemoryfunctions(nullptr, nullptr, nullptr);

    return _this;
}

SHARED void __thiscall UI_StateDestroy(LuaState *_this)
{
    LogF("UI_StateDestroy: %p", _this);
    void *memData = lua_getMemData(_this->m_state);
    _this->~LuaState();
    if (memData)
    {
        delete (MemoryPool *)memData;
    }

    LogF("Distribution:");
    for (int i = 0; i < 512; i++)
    {
        LogF("%d: %d", i, distrib[i]);
    }
}

SHARED LuaState *__thiscall SIM_StateCreate(LuaState *_this, StandardLibraries libs)
{
    LogF("SIM_StateCreate: %p", _this);

    MemoryPool *pool = new (std::nothrow) MemoryPool(0x10000000);

    // lua_setdefaultmemoryfunctions(my_ReallocFunction, my_FreeFunction, pool);

    new (_this) LuaState(libs);

    // lua_setdefaultmemoryfunctions(nullptr, nullptr, nullptr);

    return _this;
}

SHARED void __thiscall SIM_StateDestroy(LuaState *_this)
{
    LogF("SIM_StateDestroy: %p", _this);
    void *memData = lua_getMemData(_this->m_state);
    _this->~LuaState();
    if (memData)
    {
        delete (MemoryPool *)memData;
    }
}
