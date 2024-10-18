#include "Selection.h"
#include "Iterators.h"
#include <exception>

SHARED bool use_selector;

void __stdcall HandleNewSelection(Moho::CWldSession *session, Moho::UserUnitMap *new_selection)
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

            new_selection->Add(u);
        }
    }
    catch (const std::exception &e)
    {
        WarningF("%s", e.what());
    }
}