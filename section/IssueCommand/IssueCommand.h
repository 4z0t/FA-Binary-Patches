#pragma once
#include "moho.h"

struct Quaternion
{
    float x;
    float y;
    float z;
    float w;
};

namespace Moho
{

    struct EntitySetData
    {
        uint32_t *begin;
        uint32_t *end;
        uint32_t *capacity_end;
        uint32_t *inlined;
    };

    struct CellData
    {
        EntitySetData data;
        uint32_t inlined_set_items[2];

        CellData()
        {
            data.begin = inlined_set_items;
            data.end = inlined_set_items;
            data.capacity_end = inlined_set_items + 2;
            data.inlined = inlined_set_items;
        }

        ~CellData()
        {
            if (data.begin != data.inlined)
            {
                free(data.begin);
                uint32_t *inlined = data.inlined;
                data.begin = inlined;
                data.capacity_end = (uint32_t *)*inlined;
            }
            data.end = data.begin;
        }
    };

    struct EntitySet
    {
        EntitySet *next;
        EntitySet *prev;
        EntitySetData data;
        uint32_t inlined_set_items[2];

        EntitySet()
        {
            prev = this;
            next = this;
            data.begin = inlined_set_items;
            data.end = inlined_set_items;
            data.capacity_end = (uint32_t *)(inlined_set_items + 2);
            data.inlined = inlined_set_items;
        }

        bool IsEmpty() { return data.begin == data.end; }

        ~EntitySet()
        {
            if (data.begin != data.inlined)
            {
                free(data.begin);
                uint32_t *inlined = data.inlined;
                data.begin = inlined;
                data.capacity_end = (uint32_t *)*inlined;
            }
            data.end = data.begin;
            next->prev = prev;
            prev->next = next;
            prev = this;
            next = this;
        }
    };

    enum TargetType
    {
        AITARGET_None = 0,
        AITARGET_Entity = 2,
        AITARGET_Ground = 2,
    };

    enum EUnitCommandType
    {
        UNITCOMMAND_None = 0x0,
        UNITCOMMAND_Stop = 0x1,
        UNITCOMMAND_Move = 0x2,
        UNITCOMMAND_Dive = 0x3,
        UNITCOMMAND_FormMove = 0x4,
        UNITCOMMAND_BuildSiloTactical = 0x5,
        UNITCOMMAND_BuildSiloNuke = 0x6,
        UNITCOMMAND_BuildFactory = 0x7,
        UNITCOMMAND_BuildMobile = 0x8,
        UNITCOMMAND_BuildAssist = 0x9,
        UNITCOMMAND_Attack = 0xA,
        UNITCOMMAND_FormAttack = 0xB,
        UNITCOMMAND_Nuke = 0xC,
        UNITCOMMAND_Tactical = 0xD,
        UNITCOMMAND_Teleport = 0xE,
        UNITCOMMAND_Guard = 0xF,
        UNITCOMMAND_Patrol = 0x10,
        UNITCOMMAND_Ferry = 0x11,
        UNITCOMMAND_FormPatrol = 0x12,
        UNITCOMMAND_Reclaim = 0x13,
        UNITCOMMAND_Repair = 0x14,
        UNITCOMMAND_Capture = 0x15,
        UNITCOMMAND_TransportLoadUnits = 0x16,
        UNITCOMMAND_TransportReverseLoadUnits = 0x17,
        UNITCOMMAND_TransportUnloadUnits = 0x18,
        UNITCOMMAND_TransportUnloadSpecificUnits = 0x19,
        UNITCOMMAND_DetachFromTransport = 0x1A,
        UNITCOMMAND_Upgrade = 0x1B,
        UNITCOMMAND_Script = 0x1C,
        UNITCOMMAND_AssistCommander = 0x1D,
        UNITCOMMAND_KillSelf = 0x1E,
        UNITCOMMAND_DestroySelf = 0x1F,
        UNITCOMMAND_Sacrifice = 0x20,
        UNITCOMMAND_Pause = 0x21,
        UNITCOMMAND_OverCharge = 0x22,
        UNITCOMMAND_AggressiveMove = 0x23,
        UNITCOMMAND_FormAggressiveMove = 0x24,
        UNITCOMMAND_AssistMove = 0x25,
        UNITCOMMAND_SpecialAction = 0x26,
        UNITCOMMAND_Dock = 0x27,
    };

    struct TargetData
    {
        TargetType type;
        uint32_t entity_id;
        Vector3f position;

        inline static TargetData Ground(Vector3f pos)
        {
            return {AITARGET_Ground, 0xF0000000, pos};
        }

        ~TargetData() = default;
    };

    class SSTICommandIssueData
    {
    public:
        int v0;
        int v1;
        int index;
        EUnitCommandType commandType;
        TargetData target_data;
        int v9;
        int v10;
        int v11;
        int v12;
        int v13;
        int v14;
        Quaternion v15;
        float v19;
        void *blueprint;
        int v21;
        Moho::CellData cells;
        int v28;
        int v29;
        LuaObject lobj;
        void *luaState;
        int v36;
        int v37;

    private:
        static SSTICommandIssueData *__thiscall __SSTICommandIssueData(EUnitCommandType commadType, SSTICommandIssueData *_this) asm("0x00552550");

        static void *__stdcall Dtor__SSTICommandIssueData(SSTICommandIssueData *_this) asm("0x0057ABB0");

    public:
        SSTICommandIssueData(EUnitCommandType commandType)
        {
            __SSTICommandIssueData(commandType, this);
        }

        ~SSTICommandIssueData() { Dtor__SSTICommandIssueData(this); }
    };

    struct AddResult
    {
        uint32_t *add_location;
        bool resized;
    };

    // requires dtor!!!
    struct CAiTarget
    {
        int targetType;
        void *next;
        void *prev;
        Vector3f position;
        int targetPoint;
        bool targetIsMobile;
    };

} // namespace Moho

VALIDATE_SIZE(Moho::EntitySetData, 0x10);
VALIDATE_SIZE(Moho::EntitySet, 0x20);
VALIDATE_SIZE(Moho::SSTICommandIssueData, 0x98);

bool __thiscall Unit__CanBuild(void *unit, void *blueprint) asm("0x006A9E50");

Moho::EntitySet *__cdecl CheckUnitList(Moho::EntitySet *units, LuaStackObject *stackobject, LuaState *luaState, const char *funcName) noexcept(false) asm("0x006EEE40");

Moho::CellData *__cdecl CreateCellsFromTable(Moho::CellData *a1, LuaState *luaState, LuaState *luaState2, int index) asm("0x006EF270");

SHARED
{
    void UNIT_IssueCommand(Moho::EntitySet * units, void *moho, Moho::SSTICommandIssueData *command, int a5);
    void *GetBlueprintByName(LuaState * ls, int index, const char *funcName) noexcept(false);
    Moho::AddResult *EntitySetAddItem(Moho::EntitySet * set, Moho::AddResult * insert_res, void *unit);
    Moho::CellData *CopySetData(Moho::CellData * to, Moho::CellData * from);
    Moho::CAiTarget *CAiTargetFromArg(Moho::CAiTarget * target, LuaState * state, const char *funcName, LuaState *ls, int index);
    Vector3f *GetTargetPos(Vector3f * res, Moho::CAiTarget * target, bool flag);
}