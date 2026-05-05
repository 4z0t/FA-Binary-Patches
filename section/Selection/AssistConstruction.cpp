void asm__AssistConstruction()
{
    asm(
        "lea     ecx, [esp+0x64];" // cmd
        "push    esi;"             // command under mouse
        "call    _CheckCanAssistCon;"
        "cmp     byte ptr [esp+0x11], 0;" // is shift for cancel prev orders (original code)
        "jmp     0x0082084C" // jump back
        :
        :
        :
    );
}