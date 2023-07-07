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
        "L05:;"
        "CMP DWORD PTR [ECX],0x0;"
        "JE L61;"
        "ADD EBX,0x1;"
        "L61:;"
        "ADD ECX,0x8;"
        "DEC EDX;"
        "JNZ L05;"
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

void lua_createtable(/*lua_State *l, int narr, int nhash*/)
{
    asm( // copied from lua_newtable
        "push    esi;"
        "mov     esi, [esp+0x8];" // lua_State
        "mov     eax, [esi+0x10];"
        "mov     ecx, [eax+0x2C];"
        "cmp     ecx, [eax+0x24];"
        "mov  edx, [esp+0x0C];" // push narr
        "mov  ecx, [esp+0x10];" // push nhash
        "push    edi;"
        "jb      short loc_90D130;"
        "cmp     dword ptr [eax+0x28], 0;"
        "jnz     short loc_90D130;"
        "push    esi;"
        "call    0x915D90;"
        "add     esp, 4;"
        "loc_90D130: ;"
        "mov     edi, [esi+8];"
        "push    ecx;" // nhash
        "push    edx;" // narr
        "push    esi;"
        "call    0x00927320;" // luaH_new
        "movzx   edx, byte ptr [eax+4];"
        "mov     [edi], edx;"
        "mov     [edi+4], eax;"
        "mov     eax, [esi+8];"
        "mov     ecx, [esi+0x14];"
        "add     esp, 0x0C;"
        "cmp     eax, [ecx+4];"
        "jb      short loc_90D173;"
        "mov     edx, [esi+0x18];"
        "sub     edx, eax;"
        "mov     edi, 8;"
        "cmp     edx, edi;"
        "jg      short loc_90D16D;"
        "push    1;"
        "push    esi;"
        "call    0x913990;"
        "add     esp, 8;"
        "loc_90D16D:;"
        "add     [esi+8], edi;"
        "pop     edi;"
        "pop     esi;"
        "ret;"
        "loc_90D173:;"
        "add     dword ptr [esi+8], 8;"
        "pop     edi;"
        "pop     esi;"
        "ret;");
}

int _CreateTable(lua_State *l)
{
    int narr = luaL_checknumber(l, 1);
    int nhash = luaL_checknumber(l, 2);
    reinterpret_cast<void (*)(lua_State *, int, int)>(&lua_createtable)(l, narr, nhash);

    // LuaState *ls = l->LuaState;
    // LuaObject tbl;
    // LuaStackObject so;
    // CLuaObject::CLuaObject2(&tbl, ls);
    // CLuaObject::AssignNewTable(&tbl, ls, narr, nhash);
    // CLuaObject::PushStack(&tbl, &so, ls);
    // CLuaObject::DLuaObject(&tbl);
    return 1;
}

// int GettTableArraySize(lua_State *l)
// {
//     if (!lua_istable(l, 1))
//     {
//         lua_pushstring(l, "table expected");
//         lua_error(l);
//     }
//     Table *t = (Table *)lua_topointer(l, 1);
//     lua_pushnumber(l, t->sizearray);
//     return 1;
// }
// int GettTableHashSize(lua_State *l)
// {
//     if (!lua_istable(l, 1))
//     {
//         lua_pushstring(l, "table expected");
//         lua_error(l);
//     }
//     Table *t = (Table *)lua_topointer(l, 1);
//     lua_pushnumber(l, t->lsizenode);
//     return 1;
// }

int GetTableArrayAndHashSizes(lua_State *l)
{
    if (!lua_istable(l, 1))
    {
        lua_pushstring(l, "table expected");
        lua_error(l);
    }
    Table *t = (Table *)lua_topointer(l, 1);
    lua_pushnumber(l, t->sizearray);
    lua_pushnumber(l, t->lsizenode);
    return 2;
}

void _CloneTable(lua_State *l)
{
}

int _Clone(lua_State *l)
{
    if (!lua_istable(l, 1))
    {
        lua_pushstring(l, "table expected");
        lua_error(l);
    }

    // LuaState *ls = l->LuaState;
    // LuaObject tbl;
    // CLuaObject::CLuaObject3(&tbl, ls, 1);
    // LuaObject cloneTbl = CLuaObject::Clone(&tbl);
    // //CLuaObject::PushStack2(&cloneTbl, l);
    // CLuaObject::DLuaObject(&tbl);
    // CLuaObject::DLuaObject(&cloneTbl);
    return 0;
}

bool __Project(float *cam, float *resultVec2, float x, float y, float z, float width, float height)
{
    float v10;
    float v11;
    v10 = 1.0 / (cam[50] * z + cam[46] * y + cam[42] * x + cam[54]);
    v11 = (cam[48] * z + cam[44] * y + cam[40] * x + cam[52]) * v10;
    resultVec2[0] = width * ((cam[47] * z + cam[43] * y + cam[39] * x + cam[51]) * v10 + 1.0f) * 0.5f;
    resultVec2[1] = (-height) * (v11 + 1.0f) * 0.5f + height;

    if (resultVec2[0] > width || resultVec2[0] < 0)
        return false;
    if (resultVec2[1] > height || resultVec2[1] < 0)
        return false;
    return true;
}

int lua_Project(lua_State *l) // camera , x,y,z
{
    if (!lua_istable(l, 1))
    {
        lua_pushstring(l, "table expected");
        lua_error(l);
    }

    float x = luaL_checknumber(l, 2);
    float y = luaL_checknumber(l, 3);
    float z = luaL_checknumber(l, 4);
    float w = luaL_checknumber(l, 5);
    float h = luaL_checknumber(l, 6);

    lua_pushlstring(l, "_c_object", 9);
    lua_gettable(l, -7);

    int *cameraObj = (int *)lua_topointer(l, -1);

    int v3 = *cameraObj - 0x1C;
    float *camera = reinterpret_cast<float *(__thiscall *)(int)>(0x7A6A00)(v3);
    // float *camera = (float *)(*(int(__thiscall **)(int))(*(unsigned int *)v3 + 8))(v3);
    // LogF("%p", *(unsigned int *)v3 + 8); // 00E3C47C sub_7A6A00

    // float arr[]{camera[50],
    //             camera[46],
    //             camera[42],
    //             camera[54],
    //             camera[48],
    //             camera[44],
    //             camera[40],
    //             camera[52],
    //             camera[47],
    //             camera[43],
    //             camera[39],
    //             camera[51]};

    // for (int i = 0; i < 12; i++)
    //     LogF("%.3f", arr[i]);

    float result[2];
    if (__Project(camera, result, x, y, z, w, h))
    {
        lua_pushnumber(l, result[0]);
        lua_pushnumber(l, result[1]);
        return 2;
    }
    return 0;
}
/*

UI_Lua LOG(GetCamera"WorldCamera")
UI_Lua reprsl(GetCamera"WorldCamera")
INFO: table: 21256A00
DEBUG: Current gametime: 00:00:30
INFO:
INFO:  - _c_object: userdata: CScriptObject* at 124FE254 = [CScriptObject at 0x1FAEE600]



UI_Lua table.project(GetCamera"WorldCamera",1,1,1)
UI_Lua LOG(table.project(GetCamera"WorldCamera",1,1,1))

INFO: 124FE254
INFO: 1FAEE61C


UI_Lua reprsl(GetSelectedUnits()[1]:GetPosition())
UI_Lua LOG(import("/lua/ui/game/worldview.lua").viewLeft:GetScreenPos(GetSelectedUnits()[1]).x)
UI_Lua LOG(import("/lua/ui/game/worldview.lua").viewLeft:GetScreenPos(GetSelectedUnits()[1]).y)


INFO:  - _c_object: userdata: CScriptObject* at 21226F88 = [CScriptObject at 0x1FAB7600]
INFO: table: 21221500

v12 1FDD7600
v3  1FAB7600
cameraObj 1FAB761C

*/

int RegTableFuncsDesc[] = {"getsize2", &GetTableSize,
                           "empty2", &IsTableEmpty,
                           "getn2", 0x00927C20,
                           "create", &_CreateTable,
                           "clone", &_Clone,
                           "project", &lua_Project,
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