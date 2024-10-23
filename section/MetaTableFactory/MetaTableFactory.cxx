#include "MetaTableFactory.h"
#include "magic_classes.h"

class TestMetaTable : public MetaTableFactory
{
    static int Index(lua_State *L)
    {
        LogF("index");
        return 0;
    }

    static int NewIndex(lua_State *L)
    {
        LogF("newindex");
        return 0;
    }

    virtual LuaObject *Create(LuaObject *metatable, LuaState *state) const override
    {
        new (metatable) LuaObject();
        metatable->AssignNewTable(state, 0, 0);
        metatable->Register("__newindex", &NewIndex, 0);
        metatable->Register("__index", &Index, 0);
        return metatable;
    }
};
const TestMetaTable testmeta{};

int TEST_METATABLE(lua_State *l)
{

    LuaObject res{};
    res.AssignNewTable(l->LuaState, 0, 0);
    LuaObject meta = testmeta.Get(l->LuaState);
    res.SetMetaTable(&meta);
    res.PushStack(l);
    return 1;
}

// UI_Lua local t = testmeta() t.a = 5 LOG(t.a) LOG(getmetatable(t))
UIRegFunc __testmeta{"testmeta", "q", TEST_METATABLE};
