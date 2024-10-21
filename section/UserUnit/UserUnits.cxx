#include "UserUnits.h"
#include "magic_classes.h"

BitSetGetResult BitSetGet_(const BitSet &set, unsigned int ordinal)
{
    unsigned int index = 32 * (set.offset + set.end - set.begin);

    unsigned int block_i = ordinal >> 5;
    if (block_i < set.offset)
        return {set, index};

    unsigned int i = block_i - set.offset;

    if (i < set.end - set.begin && ((set.begin[i] >> (ordinal & 0x1F)) & 1) != 0)
        index = ordinal;

    return {set, index};
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

    const bool is_observer = cwldsession->focusArmy < 0;
    void *focus_army = cwldsession->GetFocusArmy();

    LuaObject list;
    list.AssignNewTable(ls, 0, 0);

    int j = 1;
    for (UserEntity *entity : entities)
    {
        UserUnit *uunit = entity->IsUserUnit();
        if (!uunit)
            continue;

        if (!entity->IsSelectable())
            continue;

        void *army = GetField<void *>(uunit, 0x120);
        if (!(army == focus_army || is_observer))
            continue;

        if (category)
        {
            void *bp = uunit->GetBlueprint();
            if (bp)
            {
                unsigned int bp_ordinal = GetField<unsigned int>(bp, 0x5c);

                if (!category->data[bp_ordinal])
                {
                    continue;
                }
            }
        }
        LuaObject obj = uunit->GetLuaObject();
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