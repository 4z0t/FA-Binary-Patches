#pragma once
#include "../UserUnit/UserUnits.h"
#include "GenericStructures.h"
#include "LuaAPI.h"
#include "UserUnit.h"
#include "moho.h"

namespace Moho
{
    struct Dragger;
    struct DraggerVTable // sizeof=0x1C
    {
        Dragger *(__thiscall *dtor)(Dragger *, char);
        void(__thiscall *field_4)(Dragger *, char *);
        int f;
        int field_C;
        int field_10;
        void(__thiscall *field_8)(Dragger *, InlinedVector<UserEntity, 2> *);
        unsigned __int8(__thiscall *field_18)(Dragger *);
    };
    struct Dragger
    {
        DraggerVTable *vtable;
        int field_4;
        Moho::CWldSession *session;
        /* Moho::CMauiFrame */ void *frame;
        Vector2f mouse_screen_pos;
        Vector3f mouse_world_pos;
    };

    SHARED
    {
        void get_units_in_selection_box(UserUnitMap * a1, Dragger * a2);
        bool CanSelectUserUnit(Moho::UserEntity * entity, Moho::CWldSession * session);
    }

    struct RRuleGameRulesImpl;

    struct vtable_gamerules
    {
        void(__thiscall *dtr)(Moho::RRuleGameRulesImpl *, int);
        void *ExportToLuaState; //        void(__thiscall __noreturn *ExportToLuaState)(struct_lock *this, int);
        void *UpdateLuaState;   //   void(__userpurge *UpdateLuaState)(struct_lock *@<ecx>, double @<st0>, int);
        void *CancelExport;
        int(__thiscall *AssignNextOrdinal)(Moho::RRuleGameRulesImpl *);
        void *GetBlueprintFromOrdinal;
        void *GetFootprints;
        void *FindFootprint; //        Moho::SFootprint *(__thiscall *FindFootprint)(Moho::RRuleGameRulesImpl *, Moho::SFootprint *, const char *);
        int(__thiscall *GetUnitBlueprints)(Moho::RRuleGameRulesImpl *);
        void *GetPropBlueprints;
        void *GetProjectileBlueprints;
        void *GetMeshBlueprints;
        int(__thiscall *GetEntityBlueprint)(Moho::RRuleGameRulesImpl *, string *);
        void *GetUnitBlueprint;       // Moho::RUnitBlueprint*(__thiscall *GetUnitBlueprint)(Moho::RRuleGameRulesImpl *_this, const Moho::RResId *resid);
        void *GetPropBlueprint;       //   Moho::RPropBlueprint *(__thiscall *GetPropBlueprint)(Moho::RRuleGameRulesImpl *, std::string *);
        void *GetMeshBlueprint;       //   Moho::RMeshBlueprint *(__thiscall *GetMeshBlueprint)(Moho::RRuleGameRulesImpl *, std::string *);
        void *GetProjectileBlueprint; //   Moho::RProjectileBlueprint *(__thiscall *GetProjectileBlueprint)(Moho::RRuleGameRulesImpl *, std::string *);
        void *GetEmitterBlueprint;
        void *GetBeamBlueprint;
        void *GetTrailBlueprint;
        void *GetEffectBlueprint;
        void *GetUnitCount;
        Moho::EntityCategory *(__thiscall *GetEntityCategory)(Moho::RRuleGameRulesImpl *_this, const char *);
        Moho::EntityCategory *(__thiscall *ParseEntityCategory)(Moho::RRuleGameRulesImpl *, Moho::EntityCategory *, const char *);
        void *UpdateChecksum; // char *(__thiscall *UpdateChecksum)(int this, gpg::MD5Context *arg0, FILE *);
    };

    struct RRuleGameRulesImpl
    {
        vtable_gamerules *vtable;

        Moho::EntityCategory *GetEntityCategory(const char *name)
        {
            return vtable->GetEntityCategory(this, name);
        }
    };

    bool __cdecl MAUI_KeyIsDown(int) asm("0x0079CB70");

    bool IsInCategory(UserEntity *entity, const char *category_name)
    {
        Moho::CWldSession *session = GetField<Moho::CWldSession *>(entity, 0xC);
        Moho::RRuleGameRulesImpl *gamerules = (Moho::RRuleGameRulesImpl *)session->gamerules;
        if (gamerules == nullptr)
            return false;

        void *bp = GetField<void *>(entity, 0x48);
        if (bp == nullptr)
            return false;

        unsigned int bp_ordinal = GetField<unsigned int>(bp, 0x5c);

        Moho::EntityCategory *category = gamerules->GetEntityCategory(category_name);
        if (category == nullptr)
            return false;

        return category->data[bp_ordinal];
    }
} // namespace Moho

SHARED
{
    void __stdcall HandleNewSelection(Moho::CWldSession * session, Moho::UserUnitMap * new_selection);
}

void __stdcall SetSelection(Moho::CWldSession *session, Moho::UserUnitMap *units) asm("0x00896140");
void __cdecl handle_click_selection(Moho::CWldSession *a1, char modifiers) asm("0x00865920");