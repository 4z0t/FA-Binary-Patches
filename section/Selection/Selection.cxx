#include "Selection.h"
#include "Iterators.h"
#include <exception>

SHARED bool use_selector;

SHARED __stdcall Moho::UserUnitMap_AddResult *UserUnitMap_Add(
    Moho::UserUnitMap_AddResult *r,
    Moho::UserUnitMap *map,
    Moho::UserUnit *uunit)
{
    Moho::UserUnitMap_InsertResult insert_result;
    Moho::MapItem item;

    item.key = uunit;

    UserEntityChain **p_chain = uunit ? &uunit->chain : nullptr;
    item.value.chain = p_chain;
    // if (p_chain)
    // {
    //     item.value.next = *p_chain;
    //     *p_chain = &item.value;
    // }
    // else
    // {
    //     item.value.next = nullptr;
    // }
    UserUnitMap_AddItem(map, &item, &insert_result);
    // UserEntityChain **i = item.value.chain;
    // if (i)
    // {
    //     while (*i != &item.value)
    //     {
    //         i = &(*i)->next;
    //     }
    //     *i = item.value.next;
    // }

    r->insert_result = insert_result;
    r->map = map;
    return r;
}

void __stdcall HandleNewSelection(Moho::CWldSession *session,
                                  Moho::UserUnitMap *new_selection)
{
    using namespace Moho;

    if (MAUI_KeyIsDown(0x132) || !use_selector)
    {
        return;
    }

    size_t size = new_selection->Size();
    if (!size)
    {
        return;
    }

    auto ls = session->state;
    LuaObject units_list;
    units_list.AssignNewTable(ls, size, 0);

    int j = 1;
    for (MapNode *node : *new_selection)
    {
        void *value = node->item.value.chain;
        if (value == nullptr)
            continue;
        Moho::UserUnit *uunit = (Moho::UserUnit *)((char *)value - 8);
        if (uunit == nullptr)
            continue;

        LuaObject obj;
        GetIUnitVTable(uunit)->GetLuaObject(Offset<Moho::Unit_ *>(uunit, 0x148), &obj);
        units_list.SetObject(j, obj);
        j++;
    }
    try
    {
        lua_State *L = ls->m_state;
        LuaObject f = SCR_Import(ls, "/lua/ui/game/gamemain.lua")["HandleSelection"];
        f.PushStack(L);
        units_list.PushStack(L);
        lua_call(L, 1, 1);

        LuaObject result{ls, -1};
        lua_pop(L, 1);

        if (!result.IsTable())
        {
            ls->Error("Expected table");
        }

        new_selection->Clear();

        for (const auto &[key, value] : IPairs(result))
        {
            UserUnit *u = UserUnitFromObj(&value, ls);

            UserUnitMap_AddResult r;
            UserUnitMap_Add(&r, new_selection, u);
        }
    }
    catch (const std::exception &e)
    {
        WarningF("%s", e.what());
    }
}