#include "Selection.h"
#include "magic_classes.h"
namespace Moho
{

    MapNode **delete_UserUnitMap(UserUnitMap *a1, MapNode **a2, MapNode *a3,
                                 MapNode *a4)
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

    UserUnitMap_AddResult *UserUnitMap_Add(UserUnitMap_AddResult *a1,
                                           UserUnitMap *a2, UserUnit *uunit)
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

    int MapGetSize(UserUnitMap *a1)
    {
        int __result;
        asm("call 0x00838AE0;" : "=a"(__result) : [a1] "a"(a1) :);
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

    MapNode *map_something(MapNode **a2)
    {
        MapNode *__result;
        asm("call 0x7B4D90;" : "=a"(__result) : [a2] "d"(a2) :);
        return __result;
    }

    UserUnit *UserUnitFromObj(const LuaObject *obj, LuaState *ls)
    {
        UserUnit *result;
        asm("call 0x00822B80;" : "=a"(result) : [obj] "a"(obj), [ls] "D"(ls) :);

        return result;
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
        "call 0x00896140;" //  Moho::CWldSession::SetSelection(Moho::CWldSession
                           //  *a1, map *a2)
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
        "jmp 0x00863982;"
        :
        : [use_selector] "i"(&use_selector)
        :);
}