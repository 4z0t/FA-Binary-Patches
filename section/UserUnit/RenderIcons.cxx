#include "RenderIcons.h"

void __stdcall ExtendRenderUserUnitIcon(UserUnitIconsTextures *unit_textures,
                                        struct_IconAux *aux,
                                        const Vector2f *pos)
{
    using namespace Moho;
    UserEntity *entity = unit_textures->entity;
    UserUnit *unit = entity->IsUserUnit();
    if (!unit)
        return;

    SPtrTexture *texture = Offset<SPtrTexture *>(unit, 1000);
    if (!texture->Valid())
    {
        return;
    }

    void *batcher = aux->batcher;
    CPrimBatcher::SetTexture(batcher, texture);

    Vector3f t_r;
    Vector3f t_l;
    Vector3f b_l;
    Vector3f b_r;

    float half_width = texture->Get()->width / 2;
    float half_height = texture->Get()->height / 2;
    t_r.x = pos->x + half_width;
    t_r.y = pos->y - half_height;
    t_r.z = 0.0;
    b_r.x = pos->x + half_width;
    b_r.y = pos->y + half_height;
    b_r.z = 0.0;
    b_l.x = pos->x - half_width;
    b_l.y = pos->y + half_height;
    b_l.z = 0.0;
    t_l.x = pos->x - half_width;
    t_l.y = pos->y - half_height;
    t_l.z = 0.0;
    DrawQuad2(0xFFFFFFFF, &b_r, &t_r, &b_l, batcher, &t_l);
}

void __stdcall ExtendUserUnitCtor(Moho::UserUnit *uunit)
{
    auto texture = Offset<SPtrTexture *>(uunit, 1000);
    new (texture) SPtrTexture();
}

void __stdcall ExtendUserUnitDtor(Moho::UserUnit *uunit)
{
    auto texture = Offset<SPtrTexture *>(uunit, 1000);
    texture->~SharedPtr();
}

int SetCustomIcon(lua_State *l)
{

    if (lua_gettop(l) != 2)
    {
        l->LuaState->Error(s_ExpectedButGot, __FUNCTION__, 2, lua_gettop(l));
    }

    Result<UserUnit> r = GetCScriptObject<UserUnit>(l, 1);

    if (r.IsFail())
    {
        lua_pushstring(l, r.reason);
        lua_error(l);
        return 0;
    }
    void *unit = r.object;
    if (unit == nullptr)
        return 0;

    auto *texture = Offset<SPtrTexture *>(unit, 1000);
    texture->Release();
    if (lua_isnil(l, 2))
    {
        return 0;
    }

    const char *path = lua_tostring(l, 2);
    if (!path)
    {
        luaL_typerror(l, 2, "string");
    }

    FromFile(texture, path, 0);

    return 0;
}

UserUnitMethodReg UserUnitSetCustomIcon{
    "SetCustomIcon",
    "UserUnit:SetCustomIcon()",
    SetCustomIcon,
    s_UserUnit};