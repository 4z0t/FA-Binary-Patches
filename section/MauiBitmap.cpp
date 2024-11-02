#include "CObject.h"
#include "magic_classes.h"
#include "moho.h"

int SetColorMask(lua_State *l)
{
    if (lua_gettop(l) != 2)
    {
        l->LuaState->Error(s_ExpectedButGot, __FUNCTION__, 2, lua_gettop(l));
    }

    auto r = GetCScriptObject<CMauiBitmap>(l, 1);
    if (r.IsFail())
    {
        lua_pushstring(l, r.reason);
        lua_error(l);
        return 0;
    }
    void *bitmap = r.object;
    if (bitmap == nullptr)
        return 0;
    const char *s = lua_tostring(l, 2);
    uint32_t color;
    if (!Moho::TryConvertToColor(s, color))
    {
        luaL_error(l, s_UnknownColor, s);
        return 0;
    }
    color &= 0x00FFFFFFu;
    GetField<uint32_t>(bitmap, 244) = (GetField<uint32_t>(bitmap, 244) & 0xFF000000u) | color;
    return 0;
}
using BitmapMethodReg = UIRegFuncT<0x00E37C14, 0x00F8D7DC>;

BitmapMethodReg BitmapSetColorMaskDesc{
    "SetColorMask",
    "Bitmap:SetColorMask(color)",
    SetColorMask,
    "CMauiBitmap"};
