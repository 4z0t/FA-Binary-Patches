#include "LuaAPI.h"

extern int factory_count asm("0x010A63A8");

class MetaTableFactory;

SHARED LuaObject *MetatableFactory__Get(LuaState *state, const MetaTableFactory *factory, LuaObject *dest);

class MetaTableFactory
{
public:
    LuaObject Get(LuaState *state) const
    {
        LuaObject metatable;
        MetatableFactory__Get(state, this, &metatable);
        return metatable;
    }

    MetaTableFactory() : index{++factory_count}
    {
    }

    virtual LuaObject *Create(LuaObject *metatable, LuaState *state) const
    {
        new (metatable) LuaObject();
        metatable->AssignNewTable(state, 0, 0);
        return metatable;
    };

    const int index;
};
