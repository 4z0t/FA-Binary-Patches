#include "Allocator.h"

void *__cdecl my_ReallocFunction(void *ptr, unsigned int oldsize, unsigned int size, void *data, const char *allocName, unsigned int flags)
{
    return realloc(ptr, size);
}

void __cdecl my_FreeFunction(void *ptr, unsigned int oldsize, void *data)
{
    free(ptr);
}

SHARED LuaState *__thiscall UI_StateCreate(LuaState *_this, StandardLibraries libs)
{
    LogF("UI_StateCreate: %p", _this);

    lua_setdefaultmemoryfunctions(my_ReallocFunction, my_FreeFunction, NULL);

    new (_this) LuaState(libs);

    lua_setdefaultmemoryfunctions(NULL, NULL, NULL);

    return _this;
}

SHARED void __thiscall UI_StateDestroy(LuaState *_this)
{
    LogF("UI_StateDestroy: %p", _this);
    _this->~LuaState();
}
