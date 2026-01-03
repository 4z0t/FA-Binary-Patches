#include "LuaAPI.h"

void *__cdecl luaHelper_ReallocFunction(void *ptr, unsigned int oldsize, unsigned int size, void *data, const char *allocName, unsigned int flags) asm("0x923F20");
void __cdecl luaHelper_FreeFunction(void *ptr, unsigned int oldsize, void *data) asm("0x923F40");

typedef void *(__cdecl *lua_ReallocFunction)(void *ptr, unsigned int oldsize, unsigned int size, void *data, const char *allocName, unsigned int flags);
typedef void(__cdecl *lua_FreeFunction)(void *ptr, unsigned int oldsize, void *data);

void __cdecl lua_setdefaultmemoryfunctions(
    lua_ReallocFunction reallocFunc,
    lua_FreeFunction freeFunc,
    void *data) asm("0x923F80");

void __cdecl lua_getdefaultmemoryfunctions(lua_ReallocFunction *reallocFunc, lua_FreeFunction *freeFunc, void **data) asm("0x923F50");
struct XException
{
    int pad[10];
    int stack[32];
    int count;
    int pad2[7];
};

XException *__stdcall Moho__XException__XException(XException *data, const char *msg) asm("0x004052A0");
XException *__thiscall Moho__XException__Dtor(XException *_this, char a2) asm("0x00405370");
unsigned int __cdecl Moho__PLAT_GetCallStack(unsigned int size, void *result) asm("0x004A22B0");
string *__cdecl Moho__PLAT_FormatCallstack(string *result, int offset, int count, void *addresses) asm("0x004A26E0");
