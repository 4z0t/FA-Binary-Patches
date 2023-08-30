void IssueBuildMobileFix()
{
    asm("lea edx, [esp+0x94];"
        "call    0x6F12C0;"
        "jmp     0x6F5FA0;");
}

/*

void IssueBuildMobile()
{

        // anonymous_0     = dword ptr -158h
        // var_14C         = dword ptr -14Ch
        // var_148         = dword ptr -148h
        // var_144         = dword ptr -144h
        // var_140         = dword ptr -140h
        // var_13C         = dword ptr -13Ch
        // var_138         = dword ptr -138h
        // var_134         = dword ptr -134h
        // var_130         = dword ptr -130h
        // var_12C         = dword ptr -12Ch
        // var_128         = dword ptr -128h
        // var_124         = dword ptr -124h
        // var_120         = dword ptr -120h
        // var_110         = dword ptr -110h
        // var_10C         = dword ptr -10Ch
        // var_108         = dword ptr -108h
        // var_104         = dword ptr -104h
        // Set2            = dword ptr -0F8h
        // var_F4          = dword ptr -0F4h
        // var_F0          = dword ptr -0F0h
        // var_EC          = dword ptr -0ECh
        // var_E8          = dword ptr -0E8h
        // var_E4          = dword ptr -0E4h
        // var_E0          = byte ptr -0E0h
        // Set1          = dword ptr -0D0h
        // var_CC          = dword ptr -0CCh
        // setBegin          = dword ptr -0C8h
        // setEnd          = dword ptr -0C4h
        // var_BC          = dword ptr -0BCh
        // var_A4          = dword ptr -0A4h
        // var_A0          = dword ptr -0A0h
        // var_C           = dword ptr -0Ch
        // var_4           = dword ptr -4
        // luaState        = dword ptr  8
    asm(
        //retrieve arguments
        #pragma region
                "push    ebp;"
                "mov     ebp, esp;"
                "and     esp, 0FFFFFFF8h;"
                "mov     eax, large fs:0;"
                "push    0FFFFFFFFh;"
                "push    offset SEH_6F5B60;"
                "push    eax;"
                "mov     large fs:0, esp;"
                "sub     esp, 140h;"
                "push    ebx;"
                "push    esi;"
                "mov     esi, [ebp+arg_0];"
                "mov     eax, [esi];"
                "push    edi;"
                "mov     edi, dword_10B8164;"
                "push    eax;"
                "call    lua_gettop;"
                "add     esp, 4;"
                "cmp     eax, 4;"
                "jz      short loc_6F5BAF;"
                "push    eax;"
                "push    4;"
                "push    edi;"
                "push    offset aSExpectedDArgs ;"// "%s\n  expected %d args, but got %d";"
                "push    esi;"
                "call    LuaPlus__LuaState__Error;"

                "add     esp, 14h;"

 "loc_6F5BAF:"
                "push    offset aIssuebuildmobi ;"// "IssueBuildMobile";"
                "push    esi;"
                "lea     ecx, [esp+160h+var_148];"
                "push    ecx;"
                "lea     edx, [esp+164h+Set1];"
                "mov     ebx, 1;"
                "push    edx;"
                "mov     [esp+168h+var_148], esi;"
                "mov     [esp+168h+var_144], ebx;"
                "call    CheckUnitList;"
                "add     esp, 8;"
                "mov     eax, esp;"
                "mov     [esp+160h+var_14C], esp;"
                "mov     [esp+160h+var_4], 0;"

                "push    offset aIssuebuildmobi ;"// "IssueBuildMobile";"
                "push    esi;"
                "lea     edi, [esp+168h+var_130];"
                "mov     [eax], esi;"
                "mov     dword ptr [eax+4], 2;"
                "call    sub_6EEF60;"
                "add     esp, 10h;"
                "mov     ecx, eax;"
                "mov     byte ptr [esp+158h+var_4], bl;"
                "xor     bl, bl;"
                "lea     eax, [esp+158h+var_13C];"
                "call    sub_5E2A90;"
                "mov     byte ptr [esp+158h+var_4], bl;"
                "mov     eax, [esp+158h+var_12C];"
                "test    eax, eax;"
                "jz      short loc_6F5C43;"
                "lea     ecx, [esp+158h+var_12C];"
                "cmp     [eax], ecx;"
                "jz      short loc_6F5C3D;"
                "mov     edi, edi;"

 "loc_6F5C30:    "
                "mov     eax, [eax];"
                "add     eax, 4;"
                "lea     edx, [esp+158h+var_12C];"
                "cmp     [eax], edx;"
                "jnz     short loc_6F5C30;"

 "loc_6F5C3D:    "
                "mov     ecx, [esp+158h+var_128];"
                "mov     [eax], ecx;"

 "loc_6F5C43:    "
                "push    offset aIssuebuildmobi ;"// "IssueBuildMobile";"
                "sub     esp, 8;"
                "mov     eax, esp;"
                "mov     edi, esi;"
                "mov     [esp+164h+var_14C], esp;"
                "mov     [eax], esi;"
                "mov     dword ptr [eax+4], 3;"
                "call    GetBlueprint;"
                "add     esp, 4;"
                "mov     edi, eax;"
                "mov     eax, esp;"
                "mov     [esp+160h+var_A4], esp;"
                "lea     edx, [esp+160h+var_110];"
                "push    esi;"
                "push    edx;"
                "mov     [esp+168h+var_14C], edi;"
                "mov     [eax], esi;"
                "mov     dword ptr [eax+4], 4;"
                "call    sub_6EF270;"
                "add     esp, 10h;"
                "test    edi, edi;"
                "mov     byte ptr [esp+158h+var_4], 2;"
                "jnz     short loc_6F5D0C;" // if Blueprint
                // else
                "mov     eax, [esp+158h+var_110];"
                "cmp     eax, [esp+158h+var_104];"
                "jz      short loc_6F5CB7;"
                "push    eax;"
                "call    j_free2_40;"
                "mov     eax, [esp+15Ch+var_104];"
                "mov     [esp+15Ch+var_110], eax;"
                "mov     ecx, [eax];"
                "add     esp, 4;"
                "mov     [esp+158h+var_108], ecx;"

 "loc_6F5CB7:    "
                "mov     [esp+158h+var_10C], eax;"
                "mov     eax, [esp+158h+setBegin];"
                "cmp     eax, [esp+158h+var_BC];"
                "jz      short loc_6F5CD4;"
                "push    eax;"
                "call    j_free2_40;"
                "add     esp, 4;"

 "loc_6F5CD4: "
                "mov     edx, [esp+158h+Set1];"
                "mov     eax, [esp+158h+var_CC];"
                "mov     [edx+4], eax;"
                "mov     ecx, [esp+158h+var_CC];"
                "mov     edx, [esp+158h+Set1];"
                "mov     [ecx], edx;"
                "xor     eax, eax;"
                "mov     ecx, [esp+158h+var_C];"
                "mov     large fs:0, ecx;"
                "pop     edi;"
                "pop     esi;"
                "pop     ebx;"
                "mov     esp, ebp;"
                "pop     ebp;"
                "ret;"

#pragma endregion

 "loc_6F5D0C:  "
                // stuff with global variables
                "mov     eax, 1;"
                "test    byte ptr dword_10C7AB0, al;"
                "jnz     short loc_6F5D3F;"
                "movss   xmm0, dword_10A99A4;"
                "or      dword_10C7AB0, eax;"
                "movss   dword_10C7AA4, xmm0;" //nans?
                "movss   dword_10C7AA8, xmm0;" //nans?
                "movss   dword_10C7AAC, xmm0;" //nans?

 "loc_6F5D3F:  "
                "movss   xmm0, dword_10C7AA4;"
                "mov     ebx, [esp+158h+setBegin];"
                "movss   [esp+158h+var_130], xmm0;"
                "movss   xmm0, dword_10C7AA8;"
                "movss   [esp+158h+var_12C], xmm0;"
                "movss   xmm0, dword_10C7AAC;"
                "xor     edi, edi;"
                 // check for start of while loop to pick unit
                "cmp     ebx, [esp+158h+setEnd];"
                "movss   [esp+158h+var_128], xmm0;"
                "jz      loc_6F5FE0;"
//do{

// if ( *v10 )
//   v12 = *v10 - 8;
// else
//   v12 = 0;
///{
 "loc_6F5D80:"
                "mov     ecx, [ebx];"
                "test    ecx, ecx;"
                "jz      short loc_6F5D8B;"
                "add     ecx, 0FFFFFFF8h;"
                "jmp     short loc_6F5D8D;"
 "loc_6F5D8B:    "
                "xor     ecx, ecx;"
///}
 "loc_6F5D8D:  "
                "mov     eax, [ecx];"
                "mov     edx, [eax+14h];"
                "call    edx;"// call for unit position
                "movss   xmm0, dword ptr [eax];"
                "movss   [esp+158h+var_148], xmm0;"
                "movss   xmm0, dword ptr [eax+4];"
                "movss   [esp+158h+var_144], xmm0;"
                "movss   xmm0, dword ptr [eax+8];"
                "mov     eax, [ebx];"

//   if ( v16 )
//        v17 = 0;
//   else
//        v17 = (void *)(v15 - 8);
//
                "test    eax, eax;"
                "movss   [esp+158h+var_140], xmm0;"
                "jz      short loc_6F5DBF;"
                "add     eax, 0FFFFFFF8h;"
                "jmp     short loc_6F5DC1;"
    "loc_6F5DBF:                       "
                "xor     eax, eax;"
    "loc_6F5DC1:                       "
//

                "mov     ecx, [esp+158h+var_14C];"
                "push    ecx;"
                "mov     ecx, eax;"
                "call    Unit__CanBuild;"
                "test    al, al;"
                "jz      loc_6F5EAC;"
                //if vector is nan or ... bla bla
                "lea     esi, [esp+158h+var_148];"
                "call    Vector__NotNaN;"
                "test    al, al;"
                "jz      loc_6F5EA9;"
                "lea     esi, [esp+158h+var_130];"
                "call    Vector__NotNaN;"
                "test    al, al;"
                "movss   xmm6, [esp+158h+var_134];"
                "movss   xmm3, [esp+158h+var_148];"
                "movss   xmm4, [esp+158h+var_144];"
                "movss   xmm5, [esp+158h+var_140];"
                "jz      short loc_6F5E75;"
                "movaps  xmm0, xmm3;"
                "subss   xmm0, [esp+158h+var_13C];"
                "movaps  xmm7, xmm0;"
                "mulss   xmm7, xmm0;"
                "movaps  xmm1, xmm4;"
                "subss   xmm1, [esp+158h+var_138];"
                "movaps  xmm0, xmm1;"
                "mulss   xmm0, xmm1;"
                "addss   xmm7, xmm0;"
                "movaps  xmm2, xmm5;"
                "subss   xmm2, xmm6;"
                "movaps  xmm0, xmm2;"
                "mulss   xmm0, xmm2;"
                "addss   xmm7, xmm0;"
                "movss   xmm0, [esp+158h+var_128];"
                "movaps  xmm1, xmm0;"
                "mulss   xmm1, xmm0;"
                "movss   xmm0, [esp+158h+var_12C];"
                "movaps  xmm2, xmm0;"
                "mulss   xmm2, xmm0;"
                "movss   xmm0, [esp+158h+var_130];"
                "addss   xmm1, xmm2;"
                "movaps  xmm2, xmm0;"
                "mulss   xmm2, xmm0;"
                "addss   xmm1, xmm2;"
                "comiss  xmm1, xmm7;"
                "jbe     short loc_6F5EA9;"

     "loc_6F5E75:                       "
                //true
                "mov     eax, [ebx];"
                "test    eax, eax;"
                "subss   xmm3, [esp+158h+var_13C];"
                "subss   xmm4, [esp+158h+var_138];"
                "mov     esi, [ebp+arg_0];"
                "subss   xmm5, xmm6;"
                "movss   [esp+158h+var_130], xmm3;"
                "movss   [esp+158h+var_12C], xmm4;"
                "movss   [esp+158h+var_128], xmm5;"
                "jz      short loc_6F5EA5;"
                "lea     edi, [eax-8];"
                "jmp     short loc_6F5EAC;"


    "loc_6F5EA5:                       "
                "xor     edi, edi;"
                "jmp     short loc_6F5EAC;"

    "loc_6F5EA9:                           "
            //else
                "mov     esi, [ebp+arg_0];"

 "loc_6F5EAC:                        "
                //increment iterator
                "add     ebx, 4;"
                "cmp     ebx, [esp+158h+setEnd];"
                "jnz     loc_6F5D80;"
// }while(v10 != v54)
                // if unit
                "test    edi, edi;"
                "jz      loc_6F5FE0;"

                "lea     edx, [esp+158h+Set2];"
                "mov     eax, edx;"
                "lea     ecx, [esp+158h+var_E0];"
                "mov     [esp+158h+var_F4], edx;"
                "mov     [esp+158h+Set2], eax;"
                "mov     edx, ecx;"
                "lea     eax, [esp+158h+Set1];"
                "mov     [esp+158h+var_F0], ecx;"
                "mov     [esp+158h+var_EC], edx;"
                "mov     [esp+158h+var_E8], eax;"
                "mov     [esp+158h+var_E4], ecx;"
                "push    edi;"
                "lea     ebx, [esp+15Ch+var_148];"
                "lea     eax, [esp+15Ch+Set2];"
                "mov     byte ptr [esp+15Ch+var_4], 3;"
                "call    sub_57DDD0;" //Moho::Set::AddItem
                "movss   xmm0, [esp+158h+var_13C];"
                "movss   [esp+158h+var_128], xmm0;"
                "movss   xmm0, [esp+158h+var_138];"
                "lea     edx, [esp+158h+var_A0];"
                "movss   [esp+158h+var_124], xmm0;"
                "movss   xmm0, [esp+158h+var_134];"
                "push    edx;"
                "mov     ecx, 8;"
                "movss   [esp+15Ch+var_120], xmm0;"
                "call    sub_552550;"
                "mov     ebx, eax;"
                "mov     byte ptr [esp+158h+var_4], 4;"
                "movss   xmm0, [esp+158h+var_128];"
                "mov     dword ptr [ebx+10h], 2;"
                "mov     dword ptr [ebx+14h], 0F0000000h;"
                "movss   dword ptr [ebx+18h], xmm0;"
                "movss   xmm0, [esp+158h+var_124];"
                "lea     ecx, [esp+158h+var_110];"
                "movss   dword ptr [ebx+1Ch], xmm0;"
                "movss   xmm0, [esp+158h+var_120];"
                "lea     eax, [ebx+58h];"
                "push    ecx;"
                "movss   dword ptr [ebx+20h], xmm0;"
                "call    sub_553370;"
                "mov     edx, [esp+158h+var_14C];"
                "push    0;"
                "mov     [ebx+50h], edx;"
                "mov     esi, [esi];"
                "push    ebx;"
                "push    esi;"
                "call    lua_getglobaluserdata;"
                "add     esp, 4;"
                "push    eax;"
                "lea     edx, [esp+164h+Set2];"
                "call    ?UNIT_IssueCommand@Moho@@YAPAVCUnitCommand@1@PAVSim@1@ABV?$EntitySetTemplate@VUnit@Moho@@@1@ABUSSTICommandIssueData@1@_N3@Z ; Moho::UNIT_IssueCommand(Moho::Sim *,Moho::EntitySetTemplate<Moho::Unit> const &,Moho::SSTICommandIssueData const &,bool,bool);"
                "add     esp, 0Ch;"
                "lea     eax, [esp+158h+var_A0];"
                "push    eax;"
                "mov     byte ptr [esp+15Ch+var_4], 3;"
                "call    sub_57ABB0;"
                "mov     eax, [esp+158h+var_F0];"
                "cmp     eax, [esp+158h+var_E4];"
                "jz      short loc_6F5FCB;"
                "push    eax;"
                "call    j_free2_40;"
                "add     esp, 4;"

    "loc_6F5FCB:                        "
                //dtor for set
                "mov     ecx, [esp+158h+Set2];"
                "mov     edx, [esp+158h+var_F4];"
                "mov     [ecx+4], edx;"
                "mov     eax, [esp+158h+var_F4];"
                "mov     ecx, [esp+158h+Set2];"
                "mov     [eax], ecx;"

   "loc_6F5FE0:  "
                // we've done all we wanted
                // calling dtors and etc
                "mov     eax, [esp+158h+var_110];"
                "cmp     eax, [esp+158h+var_104];"
                "jz      short loc_6F6001;"
                "push    eax;"
                "call    j_free2_40;"
                "mov     eax, [esp+15Ch+var_104];"
                "mov     [esp+15Ch+var_110], eax;"
                "mov     edx, [eax];"
                "add     esp, 4;"
                "mov     [esp+158h+var_108], edx;"

   "loc_6F6001:                        "
                "mov     [esp+158h+var_10C], eax;"
                "mov     eax, [esp+158h+setBegin];"
                "cmp     eax, [esp+158h+var_BC];"
                "jz      short loc_6F601E;"
                "push    eax;"
                "call    j_free2_40;"
                "add     esp, 4;"

    "loc_6F601E:                      "
                "mov     eax, [esp+158h+Set1];"
                "mov     ecx, [esp+158h+var_CC];"
                "mov     [eax+4], ecx;"
                "mov     eax, [esp+158h+Set1];"
                "mov     edx, [esp+158h+var_CC];"
                "mov     ecx, [esp+158h+var_C];"
                "pop     edi;"
                "mov     [edx], eax;"
                "pop     esi;"
                "xor     eax, eax;"
                "mov     large fs:0, ecx;"
                "pop     ebx;"
                "mov     esp, ebp;"
                "pop     ebp;"
                "ret;");
}
//*/