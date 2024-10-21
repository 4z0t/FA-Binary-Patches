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

SHARED void __thiscall DraggerHandle_OVERRIDE(Moho::Dragger *dragger, char *arg0)
{
    dragger->vtable->field_4(dragger, arg0);
    char modifiers = arg0[28];
    if (!dragger->vtable->field_18(dragger))
    {
        handle_click_selection(dragger->session, modifiers);
        return;
    }

    using namespace Moho;

    UserUnitMap units_in_selection_box{};
    get_units_in_selection_box(&units_in_selection_box, dragger);
    if (modifiers & 1)
    {
    }
    else
    {
        UserUnitMap selected_units{};

        int selection_priority = INT32_MAX;
        for (MapNode *node : units_in_selection_box)
        {
            void *value = node->item.value.chain;
            if (value == nullptr)
                continue;
            Moho::UserEntity *entity = (Moho::UserEntity *)((char *)value - 8);
            if (entity == nullptr)
                continue;
            UserUnit *uunit = GetVTable(entity)->IsUserUnit2(entity);
            if (uunit == nullptr)
                continue;

            void *bp = GetIUnitVTable(uunit)->GetBlueprint(Offset<Moho::Unit_ *>(uunit, 0x148));
            int priority = GetField<int>(bp, 508);
            if (priority < 1)
                priority = 1;
            if (priority < selection_priority)
                selection_priority = priority;
        }

        for (MapNode *node : units_in_selection_box)
        {
            void *value = node->item.value.chain;
            if (value == nullptr)
                continue;
            Moho::UserEntity *entity = (Moho::UserEntity *)((char *)value - 8);
            if (entity == nullptr)
                continue;
            UserUnit *uunit = GetVTable(entity)->IsUserUnit2(entity);
            if (uunit == nullptr)
                continue;

            void *bp = GetIUnitVTable(uunit)->GetBlueprint(Offset<Moho::Unit_ *>(uunit, 0x148));
            int priority = GetField<int>(bp, 508);
            if (priority < 1)
                priority = 1;
            if (priority == selection_priority)
            {
                selected_units.Add(uunit);
            }
        }
        LogF("%i", selection_priority);
        SetSelection(dragger->session, &selected_units);
    }

    LogF("Hi");
}