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
        UserEntity *entity; // key
        void *value;        // entity + 8
        void *unk;          //?
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
        MapNode **delete_UserUnitMap(UserUnitMap * a1, MapNode * *a2, MapNode * a3, MapNode * a4);
        UserUnitMap_AddResult *UserUnitMap_Add(UserUnitMap_AddResult * a1, UserUnitMap * a2, UserUnit * uunit);
        int MapGetSize(UserUnitMap * a1);
        MapNode **MapIterate(MapNode * *output, UserUnitMap * _this, MapNode * prev_node);
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
                MapIterate(&node, map, node);
                return *this;
            }

            MapNode *&operator*()
            {
                return node;
            }
        };

        MapIterator begin()
        {
            MapIterator iter{this};
            MapIterate(&iter.node, this, root->left);
            return iter;
        }

        MapIterator end() { return {this, root}; }

        size_t Size()
        {
            size_t n = 0;
            for (MapNode *node : *this)
            {
                void *value = node->value;
                if (value == nullptr)
                    continue;
                Moho::UserUnit *uunit = (Moho::UserUnit *)((char *)value - 8);
                if (uunit == nullptr)
                    continue;
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
    };

    bool __cdecl MAUI_KeyIsDown(int) asm("0x0079CB70");
} // namespace Moho

SHARED
{
    void __stdcall HandleNewSelection(Moho::CWldSession * session,
                                      Moho::UserUnitMap * new_selection);
}