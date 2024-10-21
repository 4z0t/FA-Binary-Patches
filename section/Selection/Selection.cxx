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
    for (Moho::UserEntity *entity : *new_selection)
    {
        Moho::UserUnit *uunit = (Moho::UserUnit *)entity;

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
        int intersect = map_instersect_count(&units_in_selection_box, &dragger->session->selectedUnits);
        int count = units_in_selection_box.Size();

        if (intersect >= count)
        {
            UserUnitMap selected_units{};
            for (Moho::UserEntity *entity : dragger->session->selectedUnits)
            {
                if (units_in_selection_box.Find(entity) == units_in_selection_box.root)
                {
                    selected_units.Add(entity);
                }
            }
            SetSelection(dragger->session, &selected_units);
        }
        else
        {
            for (Moho::UserEntity *entity : dragger->session->selectedUnits)
            {
                units_in_selection_box.Add(entity);
            }
            SetSelection(dragger->session, &units_in_selection_box);
        }
    }
    else
    {
        UserUnitMap selected_units{};

        int selection_priority = INT32_MAX;
        for (Moho::UserEntity *entity : units_in_selection_box)
        {
            UserUnit *uunit = GetVTable(entity)->IsUserUnit2(entity);
            if (uunit == nullptr)
                continue;
            auto iunit = Offset<Moho::Unit_ *>(uunit, 0x148);
            void *bp = GetIUnitVTable(uunit)->GetBlueprint(iunit);
            int priority = GetField<int>(bp, 508);
            if (priority < 1)
                priority = 1;
            if (GetIUnitVTable(uunit)->IsBeingBuilt(iunit) && IsInCategory(entity, "LOWSELECTPRIO"))
                priority = 6;
            if (priority < selection_priority)
                selection_priority = priority;
        }

        for (Moho::UserEntity *entity : units_in_selection_box)
        {
            UserUnit *uunit = GetVTable(entity)->IsUserUnit2(entity);
            if (uunit == nullptr)
                continue;

            auto iunit = Offset<Moho::Unit_ *>(uunit, 0x148);
            void *bp = GetIUnitVTable(uunit)->GetBlueprint(iunit);
            int priority = GetField<int>(bp, 508);
            if (priority < 1)
                priority = 1;
            if (GetIUnitVTable(uunit)->IsBeingBuilt(iunit) && IsInCategory(entity, "LOWSELECTPRIO"))
                priority = 6;
            if (priority == selection_priority)
            {
                selected_units.Add(uunit);
            }
        }
        SetSelection(dragger->session, &selected_units);
    }
}