#include "LuaAPI.h"
#include "magic_classes.h"
#include "Exceptions.h"
#include <string>

// static_assert(offsetof(std::runtime_error, _Data) == 4 ,"Bad std::runtime_error layout");

int TEST_Exceptions(lua_State *l)
{
    throw std::runtime_error("hello world");
    return 0;
}

int TEST_Callstack(lua_State *l)
{
    static volatile long a;
    _InterlockedExchangeAdd(&a, 1);
    MohoError("test");
    return 0;
}

// int TEST_strings(lua_State *l)
// {
//     std::string s = "hello world";
//     lua_pushstring(l, s.data());
//     return 0;
// }

UIRegFunc __testthrow{"testthrow", "", TEST_Exceptions};
UIRegFunc __testerror{"testerror", "", TEST_Callstack};
// UIRegFunc __teststrings{"testerror", "", TEST_strings};