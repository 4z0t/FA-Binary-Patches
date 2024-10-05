void HookHydroCondition()
{
    asm(
        "sub     eax, 1;" // check if hydro
        "jnz 0x008701E1;" // not hydro, jump out
        // Check if we have any templates
        "push    ecx;"
        "push    edx;"
        "mov     ecx, [esp+0x260+8];"
        "sub     ecx, [esp+0x25C+8];"
        "mov     eax, 0x2E8BA2E9;"
        "imul    ecx;"
        "sar     edx, 3;"
        "mov     eax, edx;"
        "shr     eax, 0x1F;"
        "add     eax, edx;"
        // end
        "jz SNAP_HYDRO;"
        "pop edx;"
        "pop ecx;"
        "jmp 0x008701E1;"
        "SNAP_HYDRO:"
        "pop edx;"
        "pop ecx;"
        "mov eax, 0;"
        "jmp 0x0087004D;"
        :
        :
        :);
}