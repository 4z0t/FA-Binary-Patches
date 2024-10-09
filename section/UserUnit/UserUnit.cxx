#include "UserUnit.h"
#include "magic_classes.h"

BitSetGetResult BitSetGet_(const BitSet &set, unsigned int ordinal)
{

    unsigned int index;

    unsigned int i = (ordinal >> 5) - set.offset;
    if (i < set.end - set.begin && ((set.begin[i] >> (ordinal & 0x1F)) & 1) != 0)
        index = ordinal;
    else
        index = 32 * (set.offset + set.end - set.begin);

    BitSetGetResult result{set, index};
    return result;
}

int GetFocusArmyUnits(lua_State *L)
{

    using namespace Moho;

    if (cwldsession == nullptr)
        return 0;

    auto ls = L->LuaState;

    EntityCategory *category = nullptr;
    {
        LuaObject category_obj{ls, 1};
        if (!category_obj.IsNil())
        {
            category = CastEntityCategory(category_obj);
        }
    }

    InlinedVector<UserEntity *, 2> entities;
    get_session_user_entities(&entities, 256, &cwldsession->v20);

    const int focus_army_index = cwldsession->focusArmy;
    const bool is_observer = focus_army_index < 0;
    void *focus_army = is_observer
                           ? nullptr
                           : cwldsession->userArmies[focus_army_index];

    LuaObject list;
    list.AssignNewTable(ls, 0, 0);

    int j = 1;
    for (UserEntity *entity : entities)
    {
        const UserEntityVTable *vtable = GetVTable(entity);

        UserUnit *uunit = vtable->IsUserUnit2(entity);
        if (!uunit)
            continue;

        bool is_selectable = vtable->IsSelectable(uunit);
        if (!is_selectable)
            continue;

        void *army = GetField<void *>(uunit, 0x120);
        if (!(army == focus_army || is_observer))
            continue;

        auto iunit_vtable = GetIUnitVTable(uunit);
        if (category)
        {
            void *bp = iunit_vtable->GetBlueprint(Offset<Moho::Unit_ *>(uunit, 0x148));
            if (bp)
            {

                unsigned int bp_ordinal = GetField<unsigned int>(bp, 0x5c);

                unsigned int cat_index = 32 * (category->data.offset + category->data.end -
                                               category->data.begin);

                BitSetGetResult r = BitSetGet_(category->data, bp_ordinal);
                if (r.bit_index == cat_index)
                {
                    continue;
                }
            }
        }

        LuaObject obj;
        iunit_vtable->GetLuaObject(Offset<Moho::Unit_ *>(uunit, 0x148), &obj);
        list.SetObject(j, obj);
        j++;
    }

    list.PushStack(L);

    return 1;
}

// UI_Lua LOG(GetFocusArmyUnits())
// UI_Lua reprsl(GetFocusArmyUnits())
// UI_Lua reprsl(GetFocusArmyUnits(categories.COMMAND))
// UI_Lua for i=1,1000000 do GetFocusArmyUnits() end
static UIRegFunc GetFocusArmyUnitsReg{
    "GetFocusArmyUnits",
    "",
    GetFocusArmyUnits};