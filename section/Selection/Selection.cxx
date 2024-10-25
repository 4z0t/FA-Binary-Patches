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

        LuaObject obj = uunit->GetLuaObject();
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

void HandleClickSelection(Moho::CWldSession *session, char modifiers)
{
    using namespace Moho;

    Moho::UserEntity *entity_below_mouse = session->entity_below_mouse.chain ? (Moho::UserEntity *)((int)session->entity_below_mouse.chain - 8) : nullptr;

    Moho::UserUnitMap units{};
    if (entity_below_mouse == nullptr || !Moho::CanSelectUserUnit(entity_below_mouse, session))
    {
        if ((modifiers & 3) == 0) // not shift and not control
        {
            SetSelection(session, &units);
        }
        return;
    }

    Moho::UserUnit *uunit_below_mouse = entity_below_mouse->IsUserUnit();
    if (uunit_below_mouse == nullptr)
    {
        if ((modifiers & 3) == 0) // not shift and not control
        {
            SetSelection(session, &units);
        }
        return;
    }

    if (modifiers & 2) // control
    {
        if ((modifiers & 1)) // shift
        {
            if (session->selectedUnits.Contains(uunit_below_mouse))
            {
                void *bp_entity_below_mouse = GetField<void *>(entity_below_mouse, 0x48);
                // remove all units with same blueprint
                for (Moho::UserEntity *entity : session->selectedUnits)
                {
                    if (GetField<void *>(entity, 0x48) != bp_entity_below_mouse) // compare blueprints
                    {
                        units.Add(entity);
                    }
                }
                SetSelection(session, &units);
                return;
            }
            units = session->selectedUnits;
        }

        InlinedVector<UserEntity *, 2> entities;
        get_session_user_entities(&entities, 256, &session->v20);

        void *focus_army = session->GetFocusArmy();
        void *bp_entity_below_mouse = uunit_below_mouse->GetBlueprint();

        for (UserEntity *entity : entities)
        {

            UserUnit *uunit = entity->IsUserUnit();
            if (!uunit)
                continue;

            if (!entity->IsSelectable())
                continue;

            void *army = GetField<void *>(uunit, 0x120);
            if (army != focus_army)
                continue;

            if (uunit->GetBlueprint() == bp_entity_below_mouse)
            {
                units.Add(uunit);
            }
        }
        SetSelection(session, &units);
    }
    else
    {
        if (modifiers & 1) // shift
        {
            units = session->selectedUnits;
            if (units.Contains(uunit_below_mouse))
            {
                units.Remove(uunit_below_mouse);
            }
            else
            {
                units.Add(uunit_below_mouse);
            }
        }
        else
        {
            units.Add(uunit_below_mouse);
        }
        SetSelection(session, &units);
    }
}

SHARED void __thiscall DraggerHandle_OVERRIDE(Moho::Dragger *dragger, char *arg0)
{
    dragger->vtable->field_4(dragger, arg0);
    char modifiers = arg0[28];
    if (!dragger->vtable->field_18(dragger))
    {
        HandleClickSelection(dragger->session, modifiers);
        return;
    }

    using namespace Moho;

    UserUnitMap units_in_selection_box{};
    get_units_in_selection_box(&units_in_selection_box, dragger);
    if (modifiers & 1) // shift
    {
        int intersect = map_instersect_count(&units_in_selection_box, &dragger->session->selectedUnits);
        int count = units_in_selection_box.Size();

        if (intersect >= count)
        {
            UserUnitMap selected_units{};
            for (Moho::UserEntity *entity : dragger->session->selectedUnits)
            {
                if (!units_in_selection_box.Contains(entity))
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
            UserUnit *uunit = entity->IsUserUnit();
            if (uunit == nullptr)
                continue;
            if (!uunit->IsMobile() && uunit->IsUnitState(UNITSTATE_BeingUpgraded))
                continue;

            void *bp = uunit->GetBlueprint();
            int priority = GetField<int>(bp, 508);
            if (priority < 1)
                priority = 1;
            if (uunit->IsBeingBuilt() && IsInCategory(entity, "LOWSELECTPRIO"))
                priority = 6;
            if (priority < selection_priority)
                selection_priority = priority;
        }

        for (Moho::UserEntity *entity : units_in_selection_box)
        {
            UserUnit *uunit = entity->IsUserUnit();
            if (uunit == nullptr)
                continue;
            if (!uunit->IsMobile() && uunit->IsUnitState(UNITSTATE_BeingUpgraded))
                continue;

            void *bp = uunit->GetBlueprint();
            int priority = GetField<int>(bp, 508);
            if (priority < 1)
                priority = 1;
            if (uunit->IsBeingBuilt() && IsInCategory(entity, "LOWSELECTPRIO"))
                priority = 6;
            if (priority == selection_priority)
            {
                selected_units.Add(uunit);
            }
        }
        SetSelection(dragger->session, &selected_units);
    }
}

SHARED void __cdecl ProcessUnitDoubleClick_OVERRIDE(Moho::CWldSession *session, /* Moho::CameraImpl*/ void *camera)
{
    using namespace Moho;

    Moho::UserEntity *entity_below_mouse = session->entity_below_mouse.chain ? (Moho::UserEntity *)((int)session->entity_below_mouse.chain - 8) : nullptr;
    if (entity_below_mouse == nullptr)
    {
        return;
    }

    Moho::UserUnit *uunit_below_mouse = entity_below_mouse->IsUserUnit();
    if (uunit_below_mouse == nullptr)
    {
        return;
    }

    void *focus_army = session->GetFocusArmy();
    void *army = GetField<void *>(uunit_below_mouse, 0x120);
    if (army != focus_army)
    {
        return;
    }
    void *target_bp = uunit_below_mouse->GetBlueprint();

    Moho::UserUnitMap units{session->selectedUnits};

    BaseVector<UserEntityChain> *units_in_frustum = GetField<BaseVector<UserEntityChain> *(__thiscall *)(void *)>(GetField<void *>(camera, 0), 0xA4)(camera);

    for (UserEntityChain &chain : *units_in_frustum)
    {
        Moho::UserEntity *entity = chain.chain ? (Moho::UserEntity *)((int)chain.chain - 8) : nullptr;
        if (entity == nullptr)
        {
            continue;
        }

        Moho::UserUnit *uunit = entity->IsUserUnit();
        if (uunit == nullptr)
        {
            continue;
        }

        if (uunit == uunit_below_mouse)
        {
            continue;
        }

        if (uunit->IsDead() || uunit->DestroyQueued())
        {
            continue;
        }

        if (!CanSelectUserUnit(uunit, session))
        {
            continue;
        }

        if (target_bp != uunit->GetBlueprint())
        {
            continue;
        }

        if (uunit->IsUnitState(UNITSTATE_BeingUpgraded))
        {
            continue;
        }

        units.Add(uunit);
    }

    SetSelection(session, &units);
}