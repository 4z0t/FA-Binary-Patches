
#include "LuaAPI.h"
#include "moho.h"

SHARED Vector3f *__cdecl LuaObject_ToVector3f(LuaObject *tbl, Vector3f *v) {
  v->x = tbl->GetNumber(1);
  v->y = tbl->GetNumber(2);
  v->z = tbl->GetNumber(3);
  return v;
}

SHARED Vector2f *__cdecl LuaObject_ToVector2f(LuaObject *tbl, Vector2f *v) {
  v->x = tbl->GetNumber(1);
  v->y = tbl->GetNumber(2);
  return v;
}