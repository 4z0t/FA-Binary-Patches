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
    struct MapItem
    {
        UserEntity *key;
        UserEntityChain value;
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
        // UserUnitMap_AddResult *UserUnitMap_Add(UserUnitMap_AddResult * a1, UserUnitMap * a2, UserUnit * uunit);
        MapNode **MapIterate(MapNode * *output, UserUnitMap * _this, MapNode * prev_node);
        UserUnit *UserUnitFromObj(const LuaObject *obj, LuaState *ls);
        MapFindResult *map_find(Moho::UserUnit * a1, MapFindResult * a2, UserUnitMap * a3);
        int map_instersect_count(UserUnitMap * ebx0, UserUnitMap * arg0);
        UserUnitMap *map_copy_ctor(const UserUnitMap *source, UserUnitMap *dest);
        UserUnitMap_InsertResult *UserUnitMap_AddItem(UserUnitMap * a1, MapItem * a2, UserUnitMap_InsertResult * a3);
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
                void *value = node->item.value.chain;
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

    bool __cdecl MAUI_KeyIsDown(int) asm("0x0079CB70");
} // namespace Moho

SHARED
{
    void __stdcall HandleNewSelection(Moho::CWldSession * session, Moho::UserUnitMap * new_selection);
}

void __stdcall SetSelection(Moho::CWldSession *session, Moho::UserUnitMap *units) asm("0x00896140");
void __cdecl handle_click_selection(Moho::CWldSession *a1, int *modifiers) asm("0x00865920");