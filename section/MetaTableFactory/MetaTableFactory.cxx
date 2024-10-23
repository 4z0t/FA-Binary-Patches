#include "MetaTableFactory.h"
#include "magic_classes.h"

TestMetaTable testmeta{};

int TEST_METATABLE(lua_State *l)
{

    LuaObject res{};
    res.AssignNewTable(l->LuaState, 0, 0);
    LuaObject meta = testmeta.Get(l->LuaState);
    res.SetMetaTable(&meta);
    res.PushStack(l);
    return 1;
}

UIRegFunc __testmeta{"testmeta", "q", TEST_METATABLE};
