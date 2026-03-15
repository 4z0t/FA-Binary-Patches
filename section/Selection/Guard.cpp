#include "Guard.h"

void Moho__ISSUE_Command(Moho::UserUnitMap *units, Moho::SSTICommandIssueData *a2, int clear)
{
    asm(
        "push %[clear];"
        "push %[a2];"
        "call 0x008B05E0;"
        "add esp, 0x8;"
        :
        : [units] "b"(units), [a2] "g"(a2), [clear] "g"(clear)
        :);
}

void asm__FactoryGroundAssist()
{
    asm(
        "mov ecx, edi;"
        "call %[HandleGroundAssist];"
        "jmp 0x00820637;"
        :
        : [HandleGroundAssist] "i"(HandleGroundAssist)
        :);
}

void Moho__ISSUE_FactoryCommand(Moho::UserUnitMap *units, Moho::SSTICommandIssueData *data, int clear)
{
    asm(
        "push %[clear];"
        "push %[data];"
        "call 0x008B0B30;"
        "add esp, 0x8;"
        :
        : "b"(units), [data] "g"(data), [clear] "g"(clear)
        :);
}

void Moho__CFormation__Reset(CFormation *formation)
{
    asm(
        "call 0x008380E0;"
        :
        : "S"(formation)
        :);
}