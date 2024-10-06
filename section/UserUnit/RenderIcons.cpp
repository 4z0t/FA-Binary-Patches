
#include "RenderIcons.h"
#include "CObject.h"

void __stdcall ExtendUserUnitCtor(Moho::UserUnit *uunit) {
  auto texture = Offset<Moho::CPrimBatcher::Texture *>(uunit, 1000);
  texture->data = nullptr;
  texture->lock = nullptr;
}

void __stdcall ExtendUserUnitDtor(Moho::UserUnit *uunit) {
  auto texture = Offset<Moho::CPrimBatcher::Texture *>(uunit, 1000);
  texture->Release();
  texture->data = nullptr;
  texture->lock = nullptr;
}

void ExtendCtor() {
  asm("push ebp;"
      "call %[ExtendUserUnit];"
      "pop    edi;"
      "pop    esi;"
      "mov    eax, ebp;"
      "pop    ebp;"
      "jmp    0x008BF97E;"
      :
      : [ExtendUserUnit] "i"(ExtendUserUnitCtor)
      :);
}

void ExtendDtor() {
  asm("push esi;"
      "push esi;"
      "call 0x8BF9B0;"
      "call %[ExtendUserUnitDtor];"
      "ret 4;"
      :
      : [ExtendUserUnitDtor] "i"(ExtendUserUnitDtor));
}

void ExtendRenderIcon() {

  asm("lea  edi, [esp+0xC0-0x80];"
      "push edi;"
      "push [ebp+0xC];"
      "push [ebp+0x8];"
      "call %[ExtendRenderUserUnitIcon];"
      "mov     ecx, [esp+0xC0-0xC];"
      "jmp 0x0085E083;"
      :
      : [ExtendRenderUserUnitIcon] "i"(ExtendRenderUserUnitIcon)
      :);
}

Moho::CPrimBatcher::Texture *__cdecl FromFile(Moho::CPrimBatcher::Texture *a1,
                                              const char *filename,
                                              int border) asm("0x004486F0");

int SetCustomIcon(lua_State *l) {

  if (lua_gettop(l) != 2) {
    l->LuaState->Error(s_ExpectedButGot, __FUNCTION__, 2, lua_gettop(l));
  }

  Result<UserUnit> r = GetCScriptObject<UserUnit>(l, 1);

  if (r.IsFail()) {
    lua_pushstring(l, r.reason);
    lua_error(l);
    return 0;
  }
  void *unit = r.object;
  if (unit == nullptr)
    return 0;

  auto *texture = Offset<Moho::CPrimBatcher::Texture *>(unit, 1000);
  texture->Release();
  texture->data = nullptr;
  texture->lock = nullptr;
  if (lua_isnil(l, 2)) {
    return 0;
  }

  const char *path = lua_tostring(l, 2);
  if (!path) {
    luaL_typerror(l, 2, "string");
  }

  FromFile(texture, path, 0);

  return 0;
}

UserUnitMethodReg UserUnitSetCustomIcon{
    "SetCustomIcon", "UserUnit:SetCustomIcon()", SetCustomIcon, s_UserUnit};