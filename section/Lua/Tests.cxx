#include "LuaAPI.h"
#include "magic_classes.h"
#include "Exceptions.h"


// static_assert(offsetof(std::runtime_error, _Data) == 4 ,"Bad std::runtime_error layout");

int TEST_Exceptions(lua_State *l)
{
    throw std::runtime_error("hello world");
    return 0;
}

int TEST_Callstack(lua_State *l)
{
    MohoError("test");
    return 0;
}

UIRegFunc __testthrow{"testthrow", "", TEST_Exceptions};
UIRegFunc __testerror{"testerror", "", TEST_Callstack};