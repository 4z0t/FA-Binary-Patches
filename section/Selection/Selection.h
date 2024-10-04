#pragma once
#include "../UserUnit/UserUnit.h"
#include "LuaAPI.h"
#include "moho.h"

namespace Moho
{
    struct UserUnitMap;
    struct MapNode;

    enum MapNodeColor : char
    {
        BLACK = 0x0,
        RED = 0x1,
    };

    Moho::MapNode *CreateMapNode() asm("0x007B08D0");

    struct MapNode
    {
        MapNode *left;
        MapNode *parent;
        MapNode *right;
        void *key;
        void *value;
        void *field_14;
        MapNodeColor color;
        bool is_leaf;
    };

    struct UserUnitMap_AddResult
    {
        UserUnitMap *map;
        int unk;
        bool b;
    };

    SHARED
    {

        MapNode **delete_UserUnitMap(UserUnitMap * a1, MapNode * *a2, MapNode * a3,
                                     MapNode * a4);
        UserUnitMap_AddResult *UserUnitMap_Add(UserUnitMap_AddResult * a1,
                                               UserUnitMap * a2, UserUnit * uunit);
        int MapGetSize(UserUnitMap * a1);
        MapNode **MapIterate(MapNode * *output, UserUnitMap * _this, MapNode * prev_node);
        MapNode *map_something(MapNode * *a2);
        UserUnit *UserUnitFromObj(const LuaObject *obj, LuaState *ls);
    }

    struct UserUnitMap
    {
        void *unk;
        MapNode *root;
        int size;

        UserUnitMap()
        {
            root = CreateMapNode();
            root->is_leaf = 1;
            root->parent = root;
            root->left = root;
            root->right = root;
            size = 0;
        }

        ~UserUnitMap()
        {
            MapNode *node;
            delete_UserUnitMap(this, &node, root->left, root);
            free(root);
            root = nullptr;
            size = 0;
        }

        static void ReBalanceMaybe(MapNode **node)
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
    };

    bool __cdecl MAUI_KeyIsDown(int) asm("0x0079CB70");
} // namespace Moho

SHARED
{
    void __stdcall HandleNewSelection(Moho::CWldSession * session,
                                      Moho::UserUnitMap * new_selection);
}