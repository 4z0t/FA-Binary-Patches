#include "Selection.h"
SHARED Moho::MapFindResult *__stdcall UserUnitMap_Find_OVERRIDE(UserUnit *entity, Moho::MapFindResult *r, Moho::UserUnitMap *map)
{
    Moho::MapAddItem item;
    item.key = entity;
    r->node = map->Find(item);
    r->map = map;
    return r;
}

SHARED __stdcall Moho::UserUnitMap_AddResult *UserUnitMap_Add_OVERRIDE(Moho::UserUnitMap_AddResult *r, Moho::UserUnitMap *map, Moho::UserUnit *uunit)
{
    Moho::MapAddItem item;
    item.key = uunit;
    item.value = uunit ? &uunit->chain : nullptr;
    r->map = map;
    UserUnitMap_AddItem(map, &item, &r->insert_result);
    return r;
}
