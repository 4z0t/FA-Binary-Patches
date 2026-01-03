#include "LuaAPI.h"
#include "magic_classes.h"
#include "Exceptions.h"

int TEST_Exceptions(lua_State *l)
{
    throw std::runtime_error("hello world");
    return 0;
}

UIRegFunc __testthrow{"testthrow", "", TEST_Exceptions};