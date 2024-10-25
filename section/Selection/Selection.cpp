#include "Selection.h"
#include "magic_classes.h"
namespace Moho
{

    MapNode **UserUnitMap_RemoveNodes(UserUnitMap *a1, MapNode **a2, MapNode *a3, MapNode *a4)
    {
        MapNode **__result;
        asm("push %[a4];"
            "push %[a3];"
            "push %[a2];"
            "call 0x007AF740;"
            : "=a"(__result)
            : [a1] "D"(a1), [a2] "g"(a2), [a3] "g"(a3), [a4] "g"(a4)
            :);
        return __result;
    }

    MapNode **MapIterateValid(MapNode **output, UserUnitMap *_this, MapNode *prev_node)
    {
        MapNode **__result;
        asm("push %[prev_node];"
            "call 0x007B29C0;"
            : "=a"(__result)
            : [output] "D"(output), [_this] "S"(_this), [prev_node] "g"(prev_node)
            :);
        return __result;
    }

    UserUnit *UserUnitFromObj(const LuaObject *obj, LuaState *ls)
    {
        UserUnit *result;
        asm("call 0x00822B80;" : "=a"(result) : [obj] "a"(obj), [ls] "D"(ls) :);

        return result;
    }

    int map_instersect_count(UserUnitMap *ebx0, UserUnitMap *arg0)
    {
        int __result;
        asm(
            "push %[arg0];"
            "call 0x00863760;"
            "add esp, 0x4;"
            : "=a"(__result)
            : [ebx0] "b"(ebx0), [arg0] "g"(arg0)
            :);
        return __result;
    }

    UserUnitMap *map_copy_ctor(const UserUnitMap *source, UserUnitMap *dest)
    {
        UserUnitMap *__result;
        asm(
            "push %[dest];"
            "call 0x00822210;"
            : "=a"(__result)
            : [source] "a"(source), [dest] "g"(dest)
            :);
        return __result;
    }

    UserUnitMap_InsertResult *UserUnitMap_AddItem(UserUnitMap *a1, MapAddItem *a2, UserUnitMap_InsertResult *a3)
    {
        UserUnitMap_InsertResult *__result;
        asm(
            "push %[a3];"
            "call 0x007AEDC0;"
            : "=a"(__result)
            : [a1] "a"(a1), [a2] "b"(a2), [a3] "g"(a3)
            :);
        return __result;
    }

    void get_units_in_selection_box(UserUnitMap *a1, Dragger *a2)
    {
        asm(
            "call 0x00863F10;"
            :
            : [a1] "c"(a1), [a2] "S"(a2)
            :);
    }

    bool CanSelectUserUnit(Moho::UserEntity *entity, Moho::CWldSession *session)
    {
        bool __result;
        asm(
            "call 0x00865830;"
            : "=a"(__result)
            : [entity] "D"(entity), [session] "S"(session)
            :);
        return __result;
    }
    MapNode *UserUnitMap_copy(UserUnitMap *dest, const UserUnitMap *source)
    {
        MapNode *__result;
        asm(
            "call 0x00867B20;"
            : "=a"(__result)
            : [dest] "a"(dest), [source] "b"(source)
            :);
        return __result;
    }

    int UserUnitMap_RemoveItem(UserUnitMap *a1, MapAddItem *a2)
    {
        int __result;
        asm(
            "call 0x00867AC0;"
            : "=a"(__result)
            : [a1] "a"(a1), [a2] "c"(a2)
            :);
        return __result;
    }
} // namespace Moho

SHARED bool use_selector = false;
ConDescReg<bool> use_selector_concom("ui_use_selector", "", &use_selector);

void HookSelectionCondition()
{
    asm("call  0x863F10;"
        "mov eax, %[use_selector];"
        "mov al, byte ptr [eax];"
        "test al, al;"
        "jnz    0x0086391A;"
        "test   byte ptr [edi+0x1C], 1;"
        "jz     0x00863AA3;"
        "jmp    0x0086391A;"
        :
        : [use_selector] "i"(&use_selector)
        :);
}

void HookSelectionCondition2()
{
    asm("call    0x822210;"
        "mov eax, %[use_selector];"
        "mov al, byte ptr [eax];"
        "test al, al;"
        "jz    0x0086392C;" // !use_selector
        "mov     edi, [ebp+8];"
        "test    byte ptr [edi+0x1C], 1;"
        "jnz    0x0086392C;"  // shift_pressed && use_selector
        "jmp     0x00863952;" // use_selector && !shift_pressed
        :
        : [use_selector] "i"(&use_selector)
        :);
}

void HookSelection()
{
    asm("push    ecx;"
        "push    eax;"
        "push    ecx;"
        "push    eax;"
        "call %[HandleNewSelection];"
        "call 0x00896140;" //  Moho::CWldSession::SetSelection(Moho::CWldSession *a1, map *a2)
        "ret;"
        :
        : [HandleNewSelection] "i"(HandleNewSelection)
        :);
}

void HookSelectionMerge()
{
    asm(
        "call 0x0066A330;"
        "mov edx, %[use_selector];"
        "mov dl, byte ptr [edx];"
        "test dl, dl;"
        "jz 0x0086396C;" // normal flow : merging maps
        "mov     edx, [ebp+8];"
        "test    byte ptr [edx+0x1C], 1;"
        "jnz 0x0086396C;" // normal flow : merging maps
        "jmp 0x00863982;"
        :
        : [use_selector] "i"(&use_selector)
        :);
}