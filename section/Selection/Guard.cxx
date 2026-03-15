#include "Guard.h"

SHARED void __thiscall HandleGuardOrder(void *__this, UserUnit *below_mouse)
{
    int mods = GetField<int>(__this, 0x54);
    void *var_data = Offset<void *>(below_mouse, 0x50);
    VTransform *transform = Offset<VTransform *>(var_data, 0x24);

    void *params = Offset<void *>(below_mouse, 0x44);
    uint32_t id = GetField<uint32_t>(params, 0);
    LogF("Guarding: %f %f %f", transform->pos.x, transform->pos.y, transform->pos.z);

    {
        Moho::SSTICommandIssueData data{Moho::EUnitCommandType::UNITCOMMAND_Guard};

        data.target_data = Moho::TargetData::Ground(transform->pos);

        Moho__ISSUE_Command(&cwldsession->selectedUnits, &data, !(mods & 1));
    }

    {
        Moho::SSTICommandIssueData data{Moho::EUnitCommandType::UNITCOMMAND_Guard};

        data.target_data = Moho::TargetData::Entity(id);

        Moho__ISSUE_Command(&cwldsession->selectedUnits, &data, !(mods & 1));
    }

    LogF("Mods %08x", mods);
}

void __thiscall HandleGroundAssist(void *__this)
{
    int mods = GetField<int>(__this, 0x54);
    void *mouse_drag_start = Offset<void *>(__this, 0xC);
    Vector3f pos = GetField<Vector3f>(mouse_drag_start, 0x4);
    bool is_shift = mods & 1;

    CFormation *formation = GetField<CFormation *>(cwldsession, 0x400);
    Moho__CFormation__ChooseFormation(formation, &pos, &cwldsession->selectedUnits, is_shift);
    if (formation->bestFormation >= 0)
    {
        Moho::SSTICommandIssueData data{Moho::EUnitCommandType::UNITCOMMAND_Guard};
        data.bestFormation = formation->bestFormation;
        data.orientation = formation->direction;
        data.v19 = formation->v22;

        data.target_data = Moho::TargetData::Ground(pos);

        Moho__ISSUE_FactoryCommand(&cwldsession->selectedUnits, &data, !is_shift);
        Moho__CFormation__Reset(formation);
    }
}