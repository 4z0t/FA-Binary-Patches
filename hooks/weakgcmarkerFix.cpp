#include "../define.h"

asm(R"(
     arg_0           =  4;
     limit           =  8;
    .section h0; .set h0,0x00915A00 
                    push    ebp 
                    mov     ebp, [esp+4+arg_0]
                    push    esi 
                    mov     esi, eax 
                    mov     eax, [esi]
                    push    edi 
                    xor     edi, edi 
                    cmp     eax, ebx 
                    jz      short loc_915A3B 
    loc_915A11:
                    mov     cl, [eax+5]
                    movzx   edx, cl 
                    and     edx, -7  # the fixing line
                    cmp     edx, [esp+0x0C+limit]
                    jle     short loc_915A27 
                    and     cl, 0x0FE 
                    mov     [eax+5], cl 
                    mov     esi, eax 
                    jmp     short loc_915A35 
    loc_915A27:
                    mov     ecx, [eax]
                    mov     [esi], ecx 
                    mov     ecx, ebp 
                    add     edi, 1 
                    call    0x915950 
    loc_915A35:
                    mov     eax, [esi]
                    cmp     eax, ebx 
                    jnz     short loc_915A11 
    loc_915A3B:
                    mov     eax, edi 
                    pop     edi 
                    pop     esi 
                    pop     ebp 
                    ret 
)");