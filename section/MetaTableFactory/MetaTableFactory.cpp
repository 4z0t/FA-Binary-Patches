#include "MetaTableFactory.h"

LuaObject *GetMetatable(LuaState *state, MetaTableFactory *proto, LuaObject *dest)
{
    LuaObject *__result;
    asm(
        "push %[dest];"
        "push %[proto];"
        "call 0x004CCE70;"
        : "=a"(__result)
        : [state] "S"(state), [proto] "g"(proto), [dest] "g"(dest)
        :);
    return __result;
}