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
    res.SetMetaTable(meta);
    res.PushStack(l);
    return 1;
}

// UI_Lua local t = testmeta() t.a = 5 LOG(t.a) LOG(getmetatable(t))
UIRegFunc __testmeta{"testmeta", "q", TEST_METATABLE};

class Vector3Meta : public MetaTableFactory
{
    static int Index(lua_State *L)
    {
        if (!lua_istable(L, 1))
            luaL_argerror(L, 1, "Vector3 expected");
        const char *key = lua_tostring(L, 2);
        if (!key)
            luaL_argerror(L, 2, "'x', 'y', or 'z' expected");
        if (!*key || *key - 'x' > 2 || key[1])
            luaL_argerror(L, 2, "'x', 'y', or 'z' expected");
        lua_rawgeti(L, 1, *key - 'x' + 1);
        return 1;
    }

    static int NewIndex(lua_State *L)
    {
        if (!lua_istable(L, 1))
            luaL_argerror(L, 1, "Vector3 expected");
        const char *key = lua_tostring(L, 2);
        if (!key)
            luaL_argerror(L, 2, "'x', 'y', or 'z' expected");
        if (!*key || *key - 'x' > 2 || key[1])
            luaL_argerror(L, 2, "'x', 'y', or 'z' expected");
        lua_rawseti(L, 1, *key - 'x' + 1);
        return 0;
    }

    static int Add(lua_State *L)
    {
        auto ls = L->LuaState;
        LuaObject v1{ls, 1};
        LuaObject v2{ls, 2};

        if (!v1.IsTable())
            luaL_argerror(L, 1, "Vector3 expected");
        if (!v2.IsTable())
            luaL_argerror(L, 2, "Vector3 expected");

        if (v1.GetMetaTable() != v2.GetMetaTable())
            luaL_error(L, "Invalid arguments for vector addition");

        Vector3f v1_;
        v1_.x = v1.GetNumber(1);
        v1_.y = v1.GetNumber(2);
        v1_.z = v1.GetNumber(3);
        Vector3f v2_;
        v2_.x = v2.GetNumber(1);
        v2_.y = v2.GetNumber(2);
        v2_.z = v2.GetNumber(3);
        LuaObject res = New(ls, v1_ + v2_);
        res.PushStack(L);
        return 1;
    }

    virtual LuaObject *Create(LuaObject *metatable, LuaState *state) const override
    {
        new (metatable) LuaObject();
        metatable->AssignNewTable(state, 0, 0);
        metatable->Register("__newindex", &NewIndex, 0);
        metatable->Register("__index", &Index, 0);
        metatable->Register("__add", &Add, 0);
        return metatable;
    }

public:
    static LuaObject New(LuaState *ls, Vector3f vec)
    {
        LuaObject res{};
        res.AssignNewTable(ls, 3, 0);
        res.SetNumber(1, vec.x);
        res.SetNumber(2, vec.y);
        res.SetNumber(3, vec.z);
        LuaObject meta = vec3meta.Get(ls);
        res.SetMetaTable(meta);
        return res;
    }

    static const Vector3Meta vec3meta;
};

const Vector3Meta Vector3Meta::vec3meta{};


// UI_Lua local v1 = Vector3(1,2,3) local v2 = Vector3(4,5,6) local v3 = v1+v2 LOG(v3.x,v3.y,v3.z)
// UI_Lua local v1 = Vector3(1,2,3) local v2 = Vector3(4,5,6) local v3 = v1+1
// UI_Lua local v1 = Vector3(1,2,3) local v2 = Vector3(4,5,6) local v3 = v1+''
// UI_Lua local v1 = Vector3(1,2,3) local v2 = Vector3(4,5,6) local v3 = v1+nil
int Vector3(lua_State *l)
{
    int top = lua_gettop(l);
    if (top != 3)
    {
        l->LuaState->Error(s_ExpectedButGot, __FUNCTION__, 3, top);
    }

    Vector3f v;
    v.x = luaL_checknumber(l, 1);
    v.y = luaL_checknumber(l, 2);
    v.z = luaL_checknumber(l, 3);
    auto res = Vector3Meta::New(l->LuaState, v);
    res.PushStack(l);
    return 1;
}

// UI_Lua local t = testmeta() t.a = 5 LOG(t.a) LOG(getmetatable(t))
UIRegFunc vector3_reg{"Vector3", "", Vector3};