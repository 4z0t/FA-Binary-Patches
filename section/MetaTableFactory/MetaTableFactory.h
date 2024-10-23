#include "LuaAPI.h"

extern int factory_count asm("0x010A63A8");

class MetaTableFactory;

SHARED LuaObject *GetMetatable(LuaState *state, MetaTableFactory *factory, LuaObject *dest);

class MetaTableFactory
{
public:
    LuaObject Get(LuaState *state)
    {
        LuaObject metatable;
        GetMetatable(state, this, &metatable);
        return metatable;
    }

    MetaTableFactory()
    {
        factory_count++;
        index = factory_count;
    }

    // MetaTableFactory
    virtual LuaObject *Create(LuaObject *metatable, LuaState *state)
    {
        new (metatable) LuaObject();
        metatable->AssignNewTable(state, 0, 0);
        return metatable;
    };

    int index;
};

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

    virtual LuaObject *Create(LuaObject *metatable, LuaState *state) override
    {
        new (metatable) LuaObject();
        metatable->AssignNewTable(state, 0, 0);
        metatable->Register("__newindex", &NewIndex, 0);
        metatable->Register("__index", &Index, 0);
        return metatable;
    }
};