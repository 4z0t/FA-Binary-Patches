#include "include/moho.h"
void GetTableSize()
{
    asm(
        "MOV EAX,[ESI+0xC];"
        "CMP EAX,[ESI+0x8];"
        "JAE Err;"
        "CMP DWORD PTR [EAX],0x5;"
        "JNE Err;"
        "MOV EAX,[EAX+0x4];"
        "XOR EBX,EBX;"
        "MOV CL,[EAX+0x9];"
        "TEST CL,CL;"
        "JZ L2;"
        "MOV EDX,1;"
        "SHL EDX,CL;"
        "MOV ECX,[EAX+0x14];"
        "L3:;"
        "CMP DWORD PTR [ECX+0x8],0x0;"
        "JE L04;"
        "ADD EBX,0x1;"
        "L04:;"
        "ADD ECX,0x14;"
        "DEC EDX;"
        "JNZ L3;"
        "L2:;"
        "MOV EDX,[EAX+0x20];"
        "TEST EDX,EDX;"
        "JZ L7;"
        "MOV ECX,[EAX+0x10];"
        "L5:;"
        "CMP DWORD PTR [ECX],0x0;"
        "JE L6;"
        "ADD EBX,0x1;"
        "L6:;"
        "ADD ECX,0x8;"
        "DEC EDX;"
        "JNZ L5;"
        "L7:;"
        "CVTSI2SS XMM0,EBX;"
        "SUB ESP,0x4;"
        "MOVSS [ESP],XMM0;"
        "PUSH ESI;"
        "CALL 0x0090CD40;" // PushNumber
        "ADD ESP,0x8;"
        "MOV EAX,0x1;"
        "RET;"
        "Err:;"
        "XOR EAX,EAX;");
}

void IsTableEmpty()
{
    asm(
        "MOV EAX,[ESI+0xC];"
        "CMP EAX,[ESI+0x8];"
        "JAE L721;"
        "CMP DWORD PTR [EAX],0x5;"
        "JNE L721;"
        "MOV EAX,[EAX+0x4];"
        "MOV CL,[EAX+0x9];"
        "TEST CL,CL;"
        "JZ L22;"
        "MOV EDX,1;"
        "SHL EDX,CL;"
        "MOV ECX,[EAX+0x14];"
        "L32:;"
        "CMP DWORD PTR [ECX+0x8],0x0;"
        "JNE L62;"
        "ADD ECX,0x14;"
        "DEC EDX;"
        "JNZ L32;"
        "L22:;"
        "MOV EDX,[EAX+0x20];"
        "TEST EDX,EDX;"
        "JZ L721;"
        "MOV ECX,[EAX+0x10];"
        "ITE_L5:;"
        "CMP DWORD PTR [ECX],0x0;"
        "JNE L62;"
        "ADD ECX,0x8;"
        "DEC EDX;"
        "JNZ ITE_L5;"
        "L721:;"
        "PUSH 0x1;"
        "JMP L12;"
        "L62:;"
        "PUSH 0x0;"
        "L12:;"
        "PUSH ESI;"
        "CALL 0x0090CF80;" // PushBool
        "ADD ESP,0x8;"
        "MOV EAX,0x1;");
}

void lua_createtable(lua_State *l, int narr, int nhash)
{
    asm(
                "push    esi;"
                "mov     esi, [esp+4+arg_0];"
                "mov     eax, [esi+10h];"
                "mov     ecx, [eax+2Ch];"
                "cmp     ecx, [eax+24h];"
                "push    edi;"
                "jb      short loc_90D130;"
                "cmp     dword ptr [eax+28h], 0;"
                "jnz     short loc_90D130;"
                "push    esi;"
                "call    sub_915D90;"
                "add     esp, 4;"
                "loc_90D130: ;"
                "mov     edi, [esi+8];"
                "push    0;" //nhash
                "push    0;" //narr
                "push    esi;"
                "call    0x00927320;" //luaH_new
                "movzx   edx, byte ptr [eax+4];"
                "mov     [edi], edx;"
                "mov     [edi+4], eax;"
                "mov     eax, [esi+8];"
                "mov     ecx, [esi+14h];"
                "add     esp, 0Ch;"
                "cmp     eax, [ecx+4];"
                "jb      short loc_90D173;"
                "mov     edx, [esi+18h];"
                "sub     edx, eax;"
                "mov     edi, 8;"
                "cmp     edx, edi;"
                "jg      short loc_90D16D;"
                "push    1;"
                "push    esi;"
                "call    sub_913990;"
                "add     esp, 8;"
                "loc_90D16D:                            ;"
                "add     [esi+8], edi;"
                "pop     edi;"
                "pop     esi;"
                "retn;"
                "loc_90D173:                           ;"
                "add     dword ptr [esi+8], 8;"
                "pop     edi;"
                "pop     esi;"
                "retn;"
    );
           
}

int _CreateTable(lua_State *l)
{
    int narr = luaL_checknumber(l, 1);
    int nhash = luaL_checknumber(l, 2);
    lua_createtable(l, narr, nhash);
                

    // LuaState *ls = l->LuaState;
    // LuaObject tbl;
    // LuaStackObject so;
    // CLuaObject::CLuaObject2(&tbl, ls);
    // CLuaObject::AssignNewTable(&tbl, ls, narr, nhash);
    // CLuaObject::PushStack(&tbl, &so, ls);
    // CLuaObject::DLuaObject(&tbl);
    return 1;
}

int RegTableFuncsDesc[] = {"getsize2", &GetTableSize,
                           "empty2", &IsTableEmpty,
                           "getn2", 0x00927C20,
                           "create", &_CreateTable,
                           0, 0};

void RegTableFuncs()
{
    asm(
        "CALL 0x0090DE00;"
        "MOV DWORD PTR [ESP+0x8],%[RegTableFuncsDesc];"
        "MOV DWORD PTR [ESP+0xC],0x0;"
        "CALL 0x0090DE00;"
        "JMP 0x009283B6;"
        :
        : [RegTableFuncsDesc] "i"(RegTableFuncsDesc)
        :);
}