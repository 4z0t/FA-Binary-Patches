#include "IssueCommand.h"
#include "CObject.h"
#include "magic_classes.h"
#include "utility.h"
#include <cmath>

#define VAR(var_) [var_] "g"(var_)

void UNIT_IssueCommand(Moho::EntitySet *units, void *moho,
                       Moho::SSTICommandIssueData *command, int a5)
{
    asm("push %[a5];"
        "push %[command];"
        "push %[moho];"
        "call 0x006F12C0;"
        "add esp, 0xC;"
        :
        : "d"(units),
          VAR(a5),
          VAR(command),
          VAR(moho)
        : "eax");
}

void *GetBlueprintByName(LuaState *ls, int index, const char *funcName)
{
    void *bp;

    asm("push %[funcName];"
        "push %[index];"
        "push %[ls];"
        "call 0x006EF150;"
        "add esp, 0xC;"
        : "=a"(bp)
        : [ls] "D"(ls),
          VAR(index),
          VAR(funcName)
        :);

    return bp;
}

Moho::AddResult *EntitySetAddItem(Moho::EntitySet *set, Moho::AddResult *insert_res, void *unit)
{
    Moho::AddResult *result;

    asm("push %[unit];"
        "call 0x0057DDD0;"
        : "=a"(result)
        : "a"(set),
          "b"(insert_res),
          VAR(unit)
        :);

    return result;
}

Moho::CellData *CopySetData(Moho::CellData *to, Moho::CellData *from)
{
    Moho::CellData *result;

    asm("push %[from];"
        "call 0x00553370;"
        : "=a"(result)
        : "a"(to),
          VAR(from)
        :);

    return result;
}

Moho::CAiTarget *CAiTargetFromArg(
    Moho::CAiTarget *target,
    LuaState *state,
    const char *funcName,
    LuaState *ls,
    int index)
{
    Moho::CAiTarget *result;
    asm("push %[index];"
        "push %[ls];"
        "push %[funcName];"
        "push %[state];"
        "call 0x006EEF60;"
        "add esp, 0x10;"
        : "=a"(result)
        : "D"(target),
          VAR(state),
          VAR(funcName),
          VAR(ls),
          VAR(index)
        :);
    return result;
}

Vector3f *GetTargetPos(Vector3f *res, Moho::CAiTarget *target, bool flag)
{
    Vector3f *result;
    asm("call 0x005E2A90;"
        : "=a"(result)
        : "a"(res),
          "c"(target),
          "b"(flag)
        :);
    return result;
}