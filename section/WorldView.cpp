
#include "include/moho.h"
#include "include/CObject.hpp"

int ProjectMultiple(lua_State *l)
{
    if (lua_gettop(l) != 2)
    {
        l->LuaState->Error(s_ExpectedButGot, __FUNCTION__, 2, lua_gettop(l));
    }

    Result<CUIWorldView> r = GetCScriptObject<CUIWorldView>(l, 1);
    void *worldview = r.object;
    if(worldview == nullptr)
    {
        lua_pushstring(l, r.reason);
        lua_error(l);
        return 0;
    }

    void* v = (void*)((int)worldview + 284);
    void* camera = reinterpret_cast<void*(__thiscall*)(void*)>(*(int*)v + 12)(v);

    
    return 1;
}