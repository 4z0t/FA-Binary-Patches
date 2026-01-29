#include <LuaAPI.h>
namespace lua
{
    struct TObject
    {
        int tt;
        void *value;
    };

    struct Node
    {
        TObject i_key;
        TObject i_val;
        Node *next;
    };

    struct Table
    {
        /* lua::GCObject*/ void *next;
        uint8_t tt;
        uint8_t marked;
        uint16_t gap;
        uint8_t flags;
        uint8_t lsizenode;
        // padding byte
        // padding byte
        Table *metatable;
        TObject *array;
        lua::Node *node;
        lua::Node *firstfree;
        /*lua::GCObject*/ void *gclist;
        int sizearray;
    };

} // namespace lua

VALIDATE_SIZE(lua::Table, 0x24);
extern const luaL_reg original_table_funcs[] asm("0x00D47418");
SHARED int __cdecl lua_openlibtable(lua_State *L);