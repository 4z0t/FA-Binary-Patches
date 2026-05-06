#include "AssistConstruction.h"

void asm__AssistConstruction()
{
    asm(
        "lea     ecx, [esp+0x64];" // cmd
        "push    esi;"             // command under mouse
        "call    _CheckCanAssistCon;"
        "cmp     byte ptr [esp+0x11], 0;" // is shift for cancel prev orders (original code)
        "jmp     0x0082084C"              // jump back
        :
        :
        :);
}

void asm__CheckWithReclaimOrder()
{
    // edx - session
    // edi - command mode data
    asm(
        "push    eax;" // commands caps
        "push    edx;"
        "push    edi;"
        "call    _CheckReclaimPath;"
        "test    al, al;"
        "pop     eax;"
        "jz      0x008218A9;" // to reclaim order
        "jmp     0x00821EEA;"
        :
        :
        :);
}

incomplete::Command *GetCommandByID(void *command_manager, uint32_t command_id)
{
    incomplete::Command *__result;
    asm(
        "push %[command_id];"
        "call 0x008B5BB0;"
        : "=a"(__result)
        : "S"(command_manager), [command_id] "g"(command_id)
        :);
    return __result;
}