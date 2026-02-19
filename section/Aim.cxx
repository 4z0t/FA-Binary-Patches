#include "global.h"
#include "IssueCommand/IssueCommand.h"

SHARED __thiscall bool CheckNeedAddEntityVelocity(Moho::CAiTarget *cai_target)
{
    void *entity = cai_target->entity;
    if (entity == 0 || entity == (void *)4)
        return false;

    entity = Offset<void *>(entity, -4);
    int update_tick = GetField<int>(entity, 0x174);
    void *sim = GetField<void *>(entity, 0x148);
    int sim_tick = GetField<int>(sim, 0x900);
    if (sim_tick == update_tick)
        return false;

    return true;
}

SHARED __thiscall void AddVelocity(Moho::CAiTarget *cai_target,
                                   Vector3f *aim_pos,
                                   const Vector3f *velocity,
                                   const Vector3f *target_pos)
{
    *aim_pos = *target_pos + *velocity;
}