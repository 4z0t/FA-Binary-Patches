#include "Selection.h"
#include "magic_classes.h"
namespace Moho
{

    MapNode **delete_UserUnitMap(UserUnitMap *a1, MapNode **a2, MapNode *a3, MapNode *a4)
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

    UserUnitMap_AddResult *UserUnitMap_Add(UserUnitMap_AddResult *a1, UserUnitMap *a2, UserUnit *uunit)
    {
        UserUnitMap_AddResult *__result;
        asm("push %[a3];"
            "push %[a2];"
            "call 0x007AE1B0;"
            : "=a"(__result)
            : [a1] "S"(a1), [a2] "g"(a2), [a3] "g"(uunit)
            :);
        return __result;
    }

    MapNode **MapIterate(MapNode **output, UserUnitMap *_this, MapNode *prev_node)
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

    MapFindResult *map_find(Moho::UserUnit *a1, MapFindResult *a2, UserUnitMap *a3)
    {
        MapFindResult *__result;
        asm(
            "push %[a3];"
            "call 0x00867780;"
            : "=a"(__result)
            : [a1] "a"(a1), [a2] "D"(a2), [a3] "g"(a3)
            :);
        return __result;
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

    UserUnitMap *map_copy(UserUnitMap *source, UserUnitMap *dest)
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