#pragma once
#include "../UserUnit/UserUnit.h"
#include "GenericStructures.h"
#include "LuaAPI.h"
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

    struct UserUnitMap;
    struct MapNode;

    enum MapNodeColor : char
    {
        BLACK = 0x0,
        RED = 0x1,
    };

    Moho::MapNode *CreateMapNode() asm("0x007B08D0");
    struct MapItem
    {
        UserEntity *key;
        UserEntityChain value;
    };

    struct MapAddItem
    {
        UserEntity *key;
        UserEntityChain **value;
    };

    struct MapNode
    {
        MapNode *left;
        MapNode *parent;
        MapNode *right;
        MapItem item;
        MapNodeColor color;
        bool is_leaf;
    };

    struct UserUnitMap_InsertResult
    {
        MapNode *node;
        bool inserted;
    };

    struct UserUnitMap_AddResult
    {
        UserUnitMap *map;
        UserUnitMap_InsertResult insert_result;
    };

    struct MapFindResult
    {
        UserUnitMap *map;
        MapNode *node;
    };

    SHARED
    {
        MapNode **UserUnitMap_RemoveNodes(UserUnitMap * a1, MapNode * *a2, MapNode * begin, MapNode * end);
        MapNode **MapIterateValid(MapNode * *output, UserUnitMap * _this, MapNode * prev_node);
        UserUnit *UserUnitFromObj(const LuaObject *obj, LuaState *ls);
        int map_instersect_count(UserUnitMap * ebx0, UserUnitMap * arg0);
        UserUnitMap *map_copy_ctor(const UserUnitMap *source, UserUnitMap *dest);
        UserUnitMap_InsertResult *UserUnitMap_AddItem(UserUnitMap * a1, MapAddItem * a2, UserUnitMap_InsertResult * a3);
        void get_units_in_selection_box(UserUnitMap * a1, Dragger * a2);
    }

    struct UserUnitMap
    {
        void *unk;
        MapNode *root;
        int size;

        UserUnitMap(const UserUnitMap *source)
        {
            map_copy_ctor(source, this);
        }

        UserUnitMap()
        {
            root = CreateMapNode();
            root->is_leaf = 1;
            root->parent = root;
            root->left = root;
            root->right = root;
            size = 0;
        }

        void Clear()
        {
            MapNode *node;
            UserUnitMap_RemoveNodes(this, &node, root->left, root);
        }

        ~UserUnitMap()
        {
            Clear();
            free(root);
            root = nullptr;
            size = 0;
        }

        UserUnitMap_AddResult Add(UserUnit *uunit)
        {
            UserUnitMap_AddResult r;
            MapAddItem item;
            item.key = uunit;
            item.value = uunit ? &uunit->chain : nullptr;
            r.map = this;
            UserUnitMap_AddItem(this, &item, &r.insert_result);
            return r;
        }

        struct MapIterator
        {
            UserUnitMap *map;
            MapNode *node;

            bool operator!=(const MapIterator &other)
            {
                return node != other.node;
            }

            MapIterator &operator++()
            {
                UserUnitMap::NextNode(&node);
                MapIterateValid(&node, map, node); // this function guarantees us that node value is valid
                return *this;
            }

            UserEntity *operator*()
            {
                void *value = node->item.value.chain;
                Moho::UserEntity *entity = (Moho::UserEntity *)((char *)value - 8);
                return entity;
            }
        };

        MapIterator begin()
        {
            MapIterator iter{this};
            MapIterateValid(&iter.node, this, root->left);
            return iter;
        }

        MapIterator end() { return {this, root}; }

        size_t Size()
        {
            size_t n = 0;
            for (auto *entity : *this)
            {
                n++;
            }
            return n;
        }

        static void NextNode(MapNode **node)
        {
            MapNode *result = *node;

            if (result->is_leaf)
                return;

            MapNode *right = result->right;
            if (right->is_leaf)
            {
                for (result = result->parent; !result->is_leaf; result = result->parent)
                {
                    if (*node != result->right)
                        break;
                    *node = result;
                }
                *node = result;
            }
            else
            {
                for (result = right->left; !result->is_leaf; result = result->left)
                    right = result;
                *node = right;
            }
        }

        MapNode *Find(const MapItem &item)
        {
            MapNode *root = this->root;
            MapNode *parent = root->parent;
            while (!parent->is_leaf)
            {
                if (parent->item.key >= item.key)
                {
                    root = parent;
                    parent = parent->left;
                }
                else
                {
                    parent = parent->right;
                }
            }
            if (root == this->root || item.key < root->item.key)
                return this->root;
            else
                return root;
        }
    };
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

        Moho::EntityCategory *category = gamerules->vtable->GetEntityCategory(gamerules, category_name);

        return category->data[bp_ordinal];
    }
} // namespace Moho

SHARED
{
    void __stdcall HandleNewSelection(Moho::CWldSession * session, Moho::UserUnitMap * new_selection);
}

void __stdcall SetSelection(Moho::CWldSession *session, Moho::UserUnitMap *units) asm("0x00896140");
void __cdecl handle_click_selection(Moho::CWldSession *a1, char modifiers) asm("0x00865920");