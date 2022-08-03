#include "include/desync_fix_global.h"

uint32_t input_tick = 0;
uint32_t prev_tick = 0;
uint32_t init_tick = 0;

void MOHO_USER_INPUT()
{
	register int eax asm("eax");

	asm
	(
		"SetCursorPos = 0x00C0F7C0;"
		"Sleep = 0x00C0F574;"
		"WaitForSingleObject = 0x00C0F524;"
		"cmp dword ptr [0x011FD23F], 0x1;"
		"jne run_input_check;"
	);
	input_tick++;
	if((input_tick - init_tick) > 600)
	{
		if(!paused)
		{
			asm("mov dword ptr [0x011FD23F], 0xB;");
		}
	}
	asm
	(
		"mov al, 0x1;"
		"ret 0x4;"
		"run_input_check:;"
	);
	init_tick = input_tick;
	asm
	(

		"push ebp;"
		"mov ebp,esp;"
		"and esp,0xFFFFFFF8;"
		"push 0xFFFFFFFF;"
		"push 0xBC29A5;"
		"push eax;"
		"sub esp,0x150;"
		"push ebx;"
		"mov ebx,dword ptr [ebp+0x8];"
		"mov eax,dword ptr [ebx];"
		"cmp eax,0x2;"
		"push esi;"
		"push edi;"
		"mov edi,ecx;"
		"mov dword ptr [esp+0x20],edi;"
		"jne L0xABEL_0x008704F0;"
		"mov byte ptr [edi+0x275],0x1;"
		"jmp L0xABEL_0x008705BB;"
		"L0xABEL_0x008704F0:;"
		"cmp eax,0x4;"
		"jne L0xABEL_0x008705BB;"
		"cmp byte ptr [edi+0x276],0x0;"
		"mov byte ptr [edi+0x275],0x0;"
		"je L0xABEL_0x0087051F;"
		"cmp byte ptr [0x10A645F],0x0;"
		"jne L0xABEL_0x0087051F;"
		"mov ecx,dword ptr [edi+0x120];"
		"mov eax,dword ptr [ecx];"
		"mov edx,dword ptr [eax+0x74];"
		"call edx;"
		"L0xABEL_0x0087051F:;"
		"mov byte ptr [edi+0x276],0x0;"
		"cmp byte ptr [0x10A6464],0x0;"
		"jne L0xABEL_0x0087056C;"
		"cmp byte ptr [0x10C4840],0x0;"
		"je L0xABEL_0x0087056C;"
		"mov ecx,dword ptr [0x10A6450];"
		"mov byte ptr [0x10C4840],0x0;"
		"mov eax,dword ptr [ecx];"
		"mov edx,dword ptr [eax+0x34];"
		"call edx;"
		"mov cl,0x1;"
		"cmp byte ptr [eax+0x45],cl;"
		"je L0xABEL_0x00870559;"
		"mov byte ptr [eax+0x45],cl;"
		"mov byte ptr [eax+0x44],cl;"
		"L0xABEL_0x00870559:;"
		"mov eax,dword ptr [0x10C4848];"
		"mov ecx,dword ptr [0x10C4844];"
		"push eax;"
		"push ecx;"
		"call dword ptr [SetCursorPos];"
		"L0xABEL_0x0087056C:;"
		"mov edx,dword ptr [edi+0x208];"
		"mov eax,dword ptr [edx+0x400];"
		"cmp byte ptr [eax+0x10],0x0;"
		"je L0xABEL_0x008705BB;"
		"lea esi,dword ptr [esp+0x2C];"
		"call 0x7B25C0;"
		"mov dword ptr [esp+0x164],0x0;"
		"mov eax,dword ptr [edi+0x208];"
		"mov esi,dword ptr [eax+0x400];"
		"mov byte ptr [esi+0x10],0x0;"
		"call 0x8380E0;"
		"lea eax,dword ptr [esp+0x2C];"
		"mov dword ptr [esp+0x164],0xFFFFFFFF;"
		"call 0x7B2530;"
		"L0xABEL_0x008705BB:;"
		"cmp byte ptr [0x10C4840],0x0;"
		"jne L0xABEL_0x008705E5;"
		"movss xmm0,dword ptr [ebx+0x4];"
		"movss xmm1,dword ptr [ebx+0x8];"
		"lea ecx,dword ptr [esp+0x24];"
		"push ecx;"
		"push edi;"
		"movss dword ptr [esp+0x2C],xmm0;"
		"movss dword ptr [esp+0x30],xmm1;"
		"call 0x86F520;"
		"L0xABEL_0x008705E5:;"
		"mov esi,dword ptr [edi+0x208];"
		"add esi,0x4B0;"
		"push ebx;"
		"mov ecx,edi;"
		"mov dword ptr [esp+0x1C],esi;"
		"call 0x7873A0;"
		"test al,al;"
		"jne L0xABEL_0x0087094A;"
		"cmp dword ptr [edi+0x138],0x0;"
		"jg L0xABEL_0x0087094A;"
		"cmp dword ptr [edi+0x20C],0x0;"
		"mov byte ptr [edi+0x274],al;"
		"mov byte ptr [esp+0x17],0x1;"
		"je L0xABEL_0x008708AF;"

/* 		"cmp dword ptr [0x011FD23F], 0x1;"
		"je L0xABEL_0x0087110F;"	 */

		"mov edx,dword ptr [esi+0x18];"
		"and edx,0xFF000000;"
		"cmp edx,0xFF000000;"
		"je L0xABEL_0x008708AF;"
		"mov eax,dword ptr [edi+0x208];"
		"mov ecx,dword ptr [esi+0x18];"
		"mov eax,dword ptr [eax+0x3FC];"
		"push ecx;"
		"mov esi,eax;"
		"call 0x8B5BB0;"
		"test eax,eax;"
		"mov dword ptr [esp+0x10],eax;"
		"je L0xABEL_0x008708AF;"
		"lea edx,dword ptr [esp+0x38];"
		"push edx;"
		"call 0x83DDA0;"
		"add esp,0x4;"
		"lea esi,dword ptr [esp+0x2C];"
		"mov dword ptr [esp+0x164],0x1;"
		"call 0x7B25C0;"
		"mov eax,esi;"
		"mov byte ptr [esp+0x164],0x2;"
		"mov ebx,dword ptr [edi+0x208];"
		"push eax;"
		"call 0x896000;"
		"lea ebx,dword ptr [esp+0x24];"
		"mov eax,esi;"
		"call 0x7B25F0;"
		"mov eax,dword ptr [esp+0x28];"
		"cmp eax,dword ptr [esp+0x30];"
		"je L0xABEL_0x008706E9;"
		"mov edi,edi;"
		"L0xABEL_0x008706B0:;"
		"mov eax,dword ptr [eax+0x10];"
		"xor ebx,ebx;"
		"cmp eax,ebx;"
		"je L0xABEL_0x008706BE;"
		"add eax,0xFFFFFFF8;"
		"jmp L0xABEL_0x008706C0;"
		"L0xABEL_0x008706BE:;"
		"xor eax,eax;"
		"L0xABEL_0x008706C0:;"
		"mov ecx,dword ptr [esp+0x10];"
		"push eax;"
		"push ecx;"
		"call 0x8B4300;"
		"test al,al;"
		"jne L0xABEL_0x008707D5;"
		"lea edx,dword ptr [esp+0x24];"
		"push edx;"
		"call 0x7F0490;"
		"mov eax,dword ptr [esp+0x28];"
		"cmp eax,dword ptr [esp+0x30];"
		"jne L0xABEL_0x008706B0;"
		"jmp L0xABEL_0x008706EB;"
		"L0xABEL_0x008706E9:;"
		"xor ebx,ebx;"
		"L0xABEL_0x008706EB:;"
		"mov esi,dword ptr [esp+0x10];"
		"push esi;"
		"call 0x8B4140;"
		"cmp eax,0xA;"
		"je L0xABEL_0x00870709;"
		"push esi;"
		"call 0x8B4140;"
		"cmp eax,0xB;"
		"jne L0xABEL_0x008707D5;"
		"L0xABEL_0x00870709:;"
		"mov eax,dword ptr [edi+0x208];"
		"mov esi,dword ptr [esp+0x18];"
		"push eax;"
		"push ebx;"
		"lea ecx,dword ptr [esp+0xA0];"
		"push esi;"
		"push ecx;"
		"call 0x81EC00;"
		"push ebx;"
		"lea edx,dword ptr [esp+0x10C];"
		"mov byte ptr [esp+0x178],0x3;"
		"mov ecx,dword ptr [edi+0x208];"
		"push esi;"
		"push edx;"
		"call 0x81F7B0;"
		"add esp,0x1C;"
		"mov eax,0x4;"
		"mov byte ptr [esp+0x164],al;"
		"cmp dword ptr [esp+0x9C],eax;"
		"je L0xABEL_0x00870761;"
		"cmp dword ptr [esp+0xFC],eax;"
		"jne L0xABEL_0x008707A8;"
		"L0xABEL_0x00870761:;"
		"lea eax,dword ptr [esp+0x38];"
		"push 0xE42360;"
		"push eax;"
		"call 0x4207D0;"
		"fld dword ptr [0xE4F6E8];"
		"add esp,0x8;"
		"push ebx;"
		"push ecx;"
		"fstp dword ptr [esp];"
		"push 0xFFFFFFFF;"
		"push ecx;"
		"test al,al;"
		"mov dword ptr [esp+0x20],esp;"
		"lea ecx,dword ptr [esi+0x1C];"
		"mov eax,esp;"
		"je L0xABEL_0x00870796;"
		"mov dword ptr [eax],0xE48938;"
		"jmp L0xABEL_0x0087079C;"
		"L0xABEL_0x00870796:;"
		"mov dword ptr [eax],0xE48974;"
		"L0xABEL_0x0087079C:;"
		"call 0x83ECF0;"
		"add esp,0x10;"
		"mov byte ptr [esp+0x17],bl;"
		"L0xABEL_0x008707A8:;"
		"lea edx,dword ptr [esp+0xF8];"
		"mov byte ptr [esp+0x164],0x3;"
		"call 0x7EF070;"
		"lea edx,dword ptr [esp+0x98];"
		"mov byte ptr [esp+0x164],0x2;"
		"call 0x7EF070;"
		"jmp L0xABEL_0x0087085A;"
		"L0xABEL_0x008707D5:;"
		"mov esi,dword ptr [esp+0x10];"
		"push esi;"
		"call 0x8B4140;"
		"cmp eax,0x2;"
		"je L0xABEL_0x008707EF;"
		"push esi;"
		"call 0x8B4140;"
		"cmp eax,0x4;"
		"jne L0xABEL_0x0087085A;"
		"L0xABEL_0x008707EF:;"
		"mov eax,dword ptr [edi+0x208];"
		"add eax,0x4A0;"
		"push esi;"
		"push eax;"
		"call 0x81DD00;"
		"add esp,0x8;"
		"test al,al;"
		"je L0xABEL_0x0087085A;"
		"lea ecx,dword ptr [esp+0x38];"
		"push 0xE42360;"
		"push ecx;"
		"call 0x4207D0;"
		"fld dword ptr [0xE4F6E8];"
		"add esp,0x8;"
		"push ebx;"
		"push ecx;"
		"fstp dword ptr [esp];"
		"push 0xFFFFFFFF;"
		"push ecx;"
		"mov ecx,dword ptr [esp+0x28];"
		"add ecx,0x1C;"
		"test al,al;"
		"mov dword ptr [esp+0x20],esp;"
		"mov eax,esp;"
		"je L0xABEL_0x00870841;"
		"mov dword ptr [eax],0xE489B0;"
		"jmp L0xABEL_0x00870847;"
		"L0xABEL_0x00870841:;"
		"mov dword ptr [eax],0xE489E8;"
		"L0xABEL_0x00870847:;"
		"call 0x83ECF0;"
		"add esp,0x10;"
		"mov byte ptr [esp+0x17],bl;"
		"mov byte ptr [edi+0x274],0x1;"
		"L0xABEL_0x0087085A:;"
		"mov byte ptr [esp+0x164],0x1;"
		"mov eax,dword ptr [esp+0x30];"
		"mov ecx,dword ptr [eax];"
		"push eax;"
		"push ecx;"
		"lea edx,dword ptr [esp+0x18];"
		"push edx;"
		"lea edi,dword ptr [esp+0x38];"
		"call 0x7B33B0;"
		"mov eax,dword ptr [esp+0x30];"
		"push eax;"
		"call 0x957A60;"
		"add esp,0x4;"
		"lea ecx,dword ptr [esp+0x38];"
		"mov dword ptr [esp+0x30],ebx;"
		"mov dword ptr [esp+0x34],ebx;"
		"push ecx;"
		"mov dword ptr [esp+0x168],0xFFFFFFFF;"
		"call 0x81F700;"
		"cmp byte ptr [esp+0x17],bl;"
		"mov edi,dword ptr [esp+0x20];"
		"mov ebx,dword ptr [ebp+0x8];"
		"je L0xABEL_0x008708B4;"
		"L0xABEL_0x008708AF:;"
		"call 0x83EDF0;"
		"L0xABEL_0x008708B4:;"
		"mov eax,dword ptr [ebx];"

		"cmp eax,0x8;"

		"mov esi,dword ptr [esp+0x18];"
		"jne L0xABEL_0x00870963;"
		"mov edx,dword ptr [ebx+0x1C];"
		"mov ecx,esi;"
		"push edx;"
		"lea esi,dword ptr [esp+0x3C];"
		"call 0x81FC80;"
		"add esp,0x4;"
		"mov dword ptr [esp+0x164],0x7;"
		"cmp dword ptr [esp+0x38],0x0;"
		"jne L0xABEL_0x00870921;"
		"mov ecx,dword ptr [edi+0x120];"
		"mov edx,dword ptr [esp+0x18];"
		"mov eax,dword ptr [ecx];"
		"mov eax,dword ptr [eax+0x78];"
		"add edx,0x1C;"
		"push edx;"
		"call eax;"
		"mov edi,dword ptr [edi+0x120];"
		"mov edx,dword ptr [edi];"
		"cvtsi2ss xmm0,dword ptr [ebx+0xC];"
		"cvtsi2ss xmm1,dword ptr [ebx+0x10];"
		"mov eax,dword ptr [edx+0x7C];"
		"push ecx;"
		"divss xmm0,xmm1;"
		"mov ecx,edi;"
		"movss dword ptr [esp],xmm0;"
		"call eax;"
		"jmp L0xABEL_0x00870936;"
		"L0xABEL_0x00870921:;"
		"mov ecx,dword ptr [edi+0x208];"
		"push 0x0;"
		"push ecx;"
		"lea ecx,dword ptr [esp+0x40];"
		"call 0x81FCD0;"
		"add esp,0x8;"
		"L0xABEL_0x00870936:;"
		"lea edx,dword ptr [esp+0x38];"
		"mov dword ptr [esp+0x164],0xFFFFFFFF;"
		"call 0x7EF070;"
		"L0xABEL_0x0087094A:;"
		"mov al,0x1;"
		"mov ecx,dword ptr [esp+0x15C];"
		//"mov dword ptr [0x0],ecx;"
		"pop edi;"
		"pop esi;"
		"pop ebx;"
		"mov esp,ebp;"
		"pop ebp;"
		"ret 0x4;"
		"L0xABEL_0x00870963:;"
		"mov ecx,0x1;"
		"cmp eax,ecx;"
		"jne L0xABEL_0x00870B0A;"
		"push 0x20;"
		"call 0x79CB70;"
		"add esp,0x4;"
		"test al,al;"
		"je L0xABEL_0x00870A70;"
		"cmp byte ptr [0x10A645F],0x0;"
		"jne L0xABEL_0x008709A8;"
		"mov ecx,dword ptr [edi+0x120];"
		"mov edx,dword ptr [ecx];"
		"mov eax,dword ptr [edx+0x4C];"
		"call eax;"
		"fld dword ptr [0xF57AA0];"
		"fcomip st(1);"
		"fstp st(0);"
		"jbe L0xABEL_0x00870A70;"
		"L0xABEL_0x008709A8:;"
		"cmp byte ptr [edi+0x275],0x0;"
		"je L0xABEL_0x00870ADE;"
		"cmp byte ptr [edi+0x276],0x0;"
		"je L0xABEL_0x00870A55;"
		"push edi;"
		"push 0x1;"
		"call 0x86DE30;"
		"cmp byte ptr [0x10A6464],0x0;"
		"mov ecx,dword ptr [edi+0x120];"
		"je L0xABEL_0x00870A1C;"
		"movss xmm0,dword ptr [esi+0x1C];"
		"subss xmm0,dword ptr [edi+0x278];"
		"movss dword ptr [esp+0x24],xmm0;"
		"movss xmm0,dword ptr [esi+0x20];"
		"subss xmm0,dword ptr [edi+0x27C];"
		"movss dword ptr [esp+0x28],xmm0;"
		"mov edx,dword ptr [ecx];"
		"mov edx,dword ptr [edx+0x6C];"
		"lea eax,dword ptr [esp+0x24];"
		"push eax;"
		"call edx;"
		"xor eax,eax;"
		"xor ecx,ecx;"
		"mov dword ptr [0x10C484C],eax;"
		"mov dword ptr [0x10C4850],ecx;"
		"jmp L0xABEL_0x00870A5C;"
		"L0xABEL_0x00870A1C:;"
		"cvtsi2ss xmm0,dword ptr [0x10C484C];"
		"movss dword ptr [esp+0x24],xmm0;"
		"cvtsi2ss xmm0,dword ptr [0x10C4850];"
		"movss dword ptr [esp+0x28],xmm0;"
		"mov eax,dword ptr [ecx];"
		"mov eax,dword ptr [eax+0x6C];"
		"lea edx,dword ptr [esp+0x24];"
		"push edx;"
		"call eax;"
		"xor eax,eax;"
		"xor ecx,ecx;"
		"mov dword ptr [0x10C484C],eax;"
		"mov dword ptr [0x10C4850],ecx;"
		"jmp L0xABEL_0x00870A5C;"
		"L0xABEL_0x00870A55:;"
		"mov byte ptr [edi+0x276],0x1;"
		"L0xABEL_0x00870A5C:;"
		"fld dword ptr [esi+0x1C];"
		"fstp dword ptr [edi+0x278];"
		"fld dword ptr [esi+0x20];"
		"fstp dword ptr [edi+0x27C];"
		"jmp L0xABEL_0x00870ADE;"
		"L0xABEL_0x00870A70:;"
		"cmp byte ptr [edi+0x276],0x0;"
		"je L0xABEL_0x00870A8F;"
		"cmp byte ptr [0x10A645F],0x0;"
		"jne L0xABEL_0x00870A8F;"
		"mov ecx,dword ptr [edi+0x120];"
		"mov edx,dword ptr [ecx];"
		"mov eax,dword ptr [edx+0x74];"
		"call eax;"
		"L0xABEL_0x00870A8F:;"
		"mov byte ptr [edi+0x276],0x0;"
		"cmp byte ptr [0x10A6464],0x0;"
		"jne L0xABEL_0x00870ADE;"
		"cmp byte ptr [0x10C4840],0x0;"
		"je L0xABEL_0x00870ADE;"
		"mov ecx,dword ptr [0x10A6450];"
		"mov byte ptr [0x10C4840],0x0;"
		"mov edx,dword ptr [ecx];"
		"mov eax,dword ptr [edx+0x34];"
		"call eax;"
		"cmp byte ptr [eax+0x45],0x1;"
		"je L0xABEL_0x00870ACA;"
		"mov byte ptr [eax+0x45],0x1;"
		"mov byte ptr [eax+0x44],0x1;"
		"L0xABEL_0x00870ACA:;"
		"mov ecx,dword ptr [0x10C4848];"
		"mov edx,dword ptr [0x10C4844];"
		"push ecx;"
		"push edx;"
		"call dword ptr [SetCursorPos];"
		"L0xABEL_0x00870ADE:;"
		"mov edi,dword ptr [edi+0x120];"
		"mov eax,dword ptr [edi];"
		"mov edx,dword ptr [eax+0x78];"
		"add esi,0x1C;"
		"push esi;"
		"mov ecx,edi;"
		"call edx;"
		"mov al,0x1;"
		"mov ecx,dword ptr [esp+0x15C];"
		////"mov dword ptr [0x0],ecx;"
		"pop edi;"
		"pop esi;"
		"pop ebx;"
		"mov esp,ebp;"
		"pop ebp;"
		"ret 0x4;"
		"L0xABEL_0x00870B0A:;"
		"cmp eax,0x5;"
		"jne L0xABEL_0x00870BC9;"
		"cmp dword ptr [ebx+0x14],0x2;"
		"jne L0xABEL_0x00870BC9;"
		"mov eax,dword ptr [ebx+0x1C];"
		"mov ecx,dword ptr [esp+0x18];"
		"push eax;"
		"lea esi,dword ptr [esp+0x3C];"
		"call 0x81F760;"
		"add esp,0x4;"
		"mov dword ptr [esp+0x164],0x8;"
		"cmp dword ptr [esp+0x38],0x0;"
		"jne L0xABEL_0x00870B9B;"
		"push 0x20;"
		"call 0xA825B9;"
		"add esp,0x4;"
		"mov dword ptr [esp+0x10],eax;"
		"test eax,eax;"
		"mov byte ptr [esp+0x164],0x9;"
		"je L0xABEL_0x00870B7E;"
		"mov esi,dword ptr [edi+0x120];"
		"xor edx,edx;"
		"push edx;"
		"mov ecx,0x873BD0;"
		"push ecx;"
		"mov ecx,dword ptr [esp+0x20];"
		"push edi;"
		"add ecx,0x1C;"
		"push ecx;"
		"push esi;"
		"push eax;"
		"call 0x86E060;"
		"jmp L0xABEL_0x00870B80;"
		"L0xABEL_0x00870B7E:;"
		"xor eax,eax;"
		"L0xABEL_0x00870B80:;"
		"mov byte ptr [esp+0x164],0x8;"
		"mov edi,dword ptr [edi+0xFC];"
		"push ebx;"
		"mov esi,eax;"
		"call 0x78DDE0;"
		"add esp,0x4;"
		"jmp L0xABEL_0x00870BB0;"
		"L0xABEL_0x00870B9B:;"
		"mov edx,dword ptr [edi+0x208];"
		"push 0x0;"
		"push edx;"
		"lea ecx,dword ptr [esp+0x40];"
		"call 0x81FCD0;"
		"add esp,0x8;"
		"L0xABEL_0x00870BB0:;"
		"lea edx,dword ptr [esp+0x38];"
		"mov dword ptr [esp+0x164],0xFFFFFFFF;"
		"call 0x7EF070;"
		"jmp L0xABEL_0x0087110F;"
		"L0xABEL_0x00870BC9:;"

/* 		"cmp dword ptr [0x011FD23F], 0x1;"
		"je L0xABEL_0x00870FF7;"	 */

		"cmp eax,0x6;"
		"jne L0xABEL_0x00870CA4;"
		"cmp dword ptr [ebx+0x14],ecx;"
		"jne L0xABEL_0x00870CA4;"
		"mov eax,dword ptr [edi+0x208];"
		"mov eax,dword ptr [eax+0x400];"
		"cmp byte ptr [eax+0x10],0x0;"
		"je L0xABEL_0x00870C0D;"
		"mov esi,eax;"
		"L0xABEL_0x00870BEF:;"
		"call 0x838A80;"
		"mov al,0x1;"
		"mov ecx,dword ptr [esp+0x15C];"
		//"mov dword ptr [0x0],ecx;"
		"pop edi;"
		"pop esi;"
		"pop ebx;"
		"mov esp,ebp;"
		"pop ebp;"
		"ret 0x4;"
		"L0xABEL_0x00870C0D:;"
		"mov eax,dword ptr [edi+0x148];"
		"sub eax,ecx;"
		"mov dword ptr [edi+0x140],0x6;"
		"je L0xABEL_0x00870C7A;"
		"sub eax,0x3;"
		"jne L0xABEL_0x0087110F;"
		"mov eax,dword ptr [esi+0x10];"
		"test eax,eax;"
		"je L0xABEL_0x00870C61;"
		"add eax,0xFFFFFFF8;"
		"je L0xABEL_0x00870C61;"
		"mov edx,dword ptr [edi+0x11C];"
		"mov eax,dword ptr [edx+0x28];"
		"lea ecx,dword ptr [edi+0x11C];"
		"call eax;"
		"test al,al;"
		"jne L0xABEL_0x00870C61;"
		"mov ecx,dword ptr [edi+0x120];"
		"mov edx,dword ptr [edi+0x208];"
		"push ecx;"
		"push edx;"
		"call 0x865E20;"
		"add esp,0x8;"
		"L0xABEL_0x00870C61:;"
		"mov al,0x1;"
		"mov ecx,dword ptr [esp+0x15C];"
		//"mov dword ptr [0x0],ecx;"
		"pop edi;"
		"pop esi;"
		"pop ebx;"
		"mov esp,ebp;"
		"pop ebp;"
		"ret 0x4;"
		"L0xABEL_0x00870C7A:;"
		"movzx eax,byte ptr [ebx+0x1C];"
		"and eax,0xFFFFFF01;"
		"push eax;"
		"push esi;"
		"push edi;"
		"call 0x870310;"
		"mov al,0x1;"
		"mov ecx,dword ptr [esp+0x15C];"
		//"mov dword ptr [0x0],ecx;"
		"pop edi;"
		"pop esi;"
		"pop ebx;"
		"mov esp,ebp;"
		"pop ebp;"
		"ret 0x4;"
		"L0xABEL_0x00870CA4:;"
		"cmp eax,0x5;"
		"jne L0xABEL_0x00870F12;"
		"cmp dword ptr [ebx+0x14],ecx;"
		"jne L0xABEL_0x00870F1B;"
		"mov ecx,dword ptr [edi+0x208];"
		"mov esi,dword ptr [ecx+0x400];"
		"cmp byte ptr [esi+0x10],0x0;"
		"jne L0xABEL_0x00870BEF;"
		"mov dword ptr [edi+0x140],eax;"
		"mov edx,dword ptr [ebx+0x1C];"
		"mov eax,dword ptr [esp+0x18];"
		"push edx;"
		"push eax;"
		"lea edx,dword ptr [esp+0xA0];"
		"push edx;"
		"call 0x81F7B0;"
		"add esp,0xC;"
		"add edi,0x148;"
		"mov ebx,eax;"
		"mov dword ptr [esp+0x164],0xA;"
		"call 0x82B230;"
		"lea edx,dword ptr [esp+0x98];"
		"mov dword ptr [esp+0x164],0xFFFFFFFF;"
		"call 0x7EF070;"
		"mov eax,dword ptr [edi];"
		//"cmp edi,0x5;"
		//"ja L0xABEL_0x00870EDC;"
	);
		switch(eax)
		{
			case 0:
				asm("jmp L0xABEL_0x0087110F;");
				break;
			case 2:
			case 3:
				asm("jmp case3;");
				break;
			case 4:
				break;
			case 5:
				asm("jmp case5;");
				break;
			default:
				asm("jmp L0xABEL_0x00870EDC;");
				break;
		}
	asm
	(

		//"jmp L0xABEL_0x00000000;"
		"mov edi,dword ptr [esp+0x20];"
		"mov eax,dword ptr [edi+0x11C];"
		"mov edx,dword ptr [eax+0x28];"
		"lea ecx,dword ptr [edi+0x11C];"
		"call edx;"
		"test al,al;"
		"je L0xABEL_0x00870E0C;"
		"cmp dword ptr [edi+0x294],0x0;"
		"je L0xABEL_0x00870E3A;"
		"push ecx;"
		"lea ebx,dword ptr [edi+0x280];"
		"mov ecx,esp;"
		"mov dword ptr [esp+0x14],esp;"
		"push ebx;"
		"call 0x408710;"
		"call 0x7AAD00;"
		"add esp,0x4;"
		"test eax,eax;"
		"je L0xABEL_0x00870D8C;"
		"fldz;"
		"mov edx,dword ptr [eax];"
		"mov edx,dword ptr [edx+0x28];"
		"push ecx;"
		"mov ecx,dword ptr [esp+0x1C];"
		"fstp dword ptr [esp];"
		"add ecx,0x4;"
		"push ecx;"
		"mov ecx,eax;"
		"call edx;"
		"L0xABEL_0x00870D8C:;"
		"push 0x24;"
		"call 0xA825B9;"
		"mov esi,eax;"
		"add esp,0x4;"
		"mov dword ptr [esp+0x10],esi;"
		"test esi,esi;"
		"mov dword ptr [esp+0x164],0xB;"
		"je L0xABEL_0x00870DD2;"
		"sub esp,0x1C;"
		"mov ecx,esp;"
		"mov dword ptr [esp+0x3C],esp;"
		"push ebx;"
		"call 0x405590;"
		"mov byte ptr [esp+0x180],0xC;"
		"push esi;"
		"mov byte ptr [esp+0x184],0xB;"
		"call 0x86E270;"
		"jmp L0xABEL_0x00870DD4;"
		"L0xABEL_0x00870DD2:;"
		"xor eax,eax;"
		"L0xABEL_0x00870DD4:;"
		"mov ecx,dword ptr [ebp+0x8];"
		"mov dword ptr [esp+0x164],0xFFFFFFFF;"
		"mov edi,dword ptr [edi+0xFC];"
		"push ecx;"
		"mov esi,eax;"
		"call 0x78DDE0;"
		"add esp,0x4;"
		"mov al,0x1;"
		"mov ecx,dword ptr [esp+0x15C];"
		//"mov dword ptr [0x0],ecx;"
		"pop edi;"
		"pop esi;"
		"pop ebx;"
		"mov esp,ebp;"
		"pop ebp;"
		"ret 0x4;"
		"L0xABEL_0x00870E0C:;"
		"mov edx,dword ptr [ebp+0x8];"
		"mov eax,dword ptr [edi+0xFC];"
		"mov ecx,dword ptr [edi+0x120];"
		"push edx;"
		"mov edx,dword ptr [edi+0x208];"
		"push eax;"
		"call 0x865880;"
		"lea edx,dword ptr [edi+0x29C];"
		"mov ecx,eax;"
		"add esp,0x8;"
		"mov eax,edx;"
		"call 0x873810;"
		"L0xABEL_0x00870E3A:;"
		"mov al,0x1;"
		"mov ecx,dword ptr [esp+0x15C];"
		//"mov dword ptr [0x0],ecx;"
		"pop edi;"
		"pop esi;"
		"pop ebx;"
		"mov esp,ebp;"
		"pop ebp;"
		"ret 0x4;"
		"case3:;"
		"mov eax,dword ptr [esp+0x20];"
		"mov edx,dword ptr [eax+0x120];"
		"mov edi,dword ptr [eax+0xFC];"
		"lea ecx,dword ptr [eax+0x214];"
		"push ecx;"
		"mov ecx,dword ptr [ebp+0x8];"
		"push edx;"
		"mov edx,dword ptr [eax+0x208];"
		"push ecx;"
		"push edx;"
		"call 0x823CB0;"
		"add esp,0x10;"
		"mov al,0x1;"
		"mov ecx,dword ptr [esp+0x15C];"
		//"mov dword ptr [0x0],ecx;"
		"pop edi;"
		"pop esi;"
		"pop ebx;"
		"mov esp,ebp;"
		"pop ebp;"
		"ret 0x4;"
		"case5:;"
		"mov eax,dword ptr [esp+0x18];"
		"mov ecx,dword ptr [eax+0x18];"
		"mov eax,dword ptr [esp+0x20];"
		"mov edx,dword ptr [eax+0x120];"
		"mov edi,dword ptr [eax+0xFC];"
		"push ecx;"
		"mov ecx,dword ptr [ebp+0x8];"
		"push edx;"
		"mov edx,dword ptr [eax+0x208];"
		"push ecx;"
		"push edx;"
		"call 0x8242B0;"
		"add esp,0x10;"
		"mov al,0x1;"
		"mov ecx,dword ptr [esp+0x15C];"
		//"mov dword ptr [0x0],ecx;"
		"pop edi;"
		"pop esi;"
		"pop ebx;"
		"mov esp,ebp;"
		"pop ebp;"
		"ret 0x4;"
		"L0xABEL_0x00870EDC:;"
		"mov eax,dword ptr [ebp+0x8];"
		"movzx ecx,byte ptr [eax+0x1C];"
		"mov edx,dword ptr [esp+0x18];"
		"mov eax,dword ptr [esp+0x20];"
		"and ecx,0xFFFFFF01;"
		"push ecx;"
		"push edx;"
		"push eax;"
		"call 0x870310;"
		"mov al,0x1;"
		"mov ecx,dword ptr [esp+0x15C];"
		//"mov dword ptr [0x0],ecx;"
		"pop edi;"
		"pop esi;"
		"pop ebx;"
		"mov esp,ebp;"
		"pop ebp;"
		"ret 0x4;"
		"L0xABEL_0x00870F12:;"

		"cmp eax,0x6;"
		"jne L0xABEL_0x00870FF7;"
		"L0xABEL_0x00870F1B:;"
		"cmp dword ptr [ebx+0x14],0x3;"
		"jne L0xABEL_0x00870FF7;"
		"mov ecx,dword ptr [edi+0x208];"
		"push ecx;"
		"mov dword ptr [edi+0x144],eax;"
		"mov edx,dword ptr [ebx+0x1C];"
		"mov eax,dword ptr [esp+0x1C];"
		"push edx;"
		"push eax;"
		"lea ecx,dword ptr [esp+0xA4];"
		"push ecx;"
		"call 0x81EC00;"
		"add esp,0x10;"
		"lea esi,dword ptr [edi+0x1A8];"
		"mov ebx,eax;"
		"mov edi,esi;"
		"mov dword ptr [esp+0x164],0xD;"
		"call 0x82B230;"
		"lea edx,dword ptr [esp+0x98];"
		"mov dword ptr [esp+0x164],0xFFFFFFFF;"
		"call 0x7EF070;"
		"cmp dword ptr [esi],0x1;"
		"jne L0xABEL_0x0087110F;"
		"mov edi,dword ptr [esp+0x20];"
		"mov eax,dword ptr [edi+0x208];"
		"add eax,0x4A0;"
		"call 0x7B59B0;"
		"test eax,eax;"
		"jbe L0xABEL_0x0087110F;"
		"mov edx,dword ptr [ebp+0x8];"
		"mov al,byte ptr [edx+0x1C];"
		"and al,0x1;"
		"lea esi,dword ptr [esp+0x2C];"
		"mov byte ptr [esp+0x10],al;"
		"call 0x7B25C0;"
		"mov ecx,esi;"
		"mov dword ptr [esp+0x164],0xE;"
		"mov ebx,dword ptr [edi+0x208];"
		"push ecx;"
		"call 0x896000;"
		"mov edx,dword ptr [edi+0x208];"
		"mov eax,dword ptr [edx+0x400];"
		"mov ecx,dword ptr [esp+0x10];"
		"mov edx,dword ptr [esp+0x18];"
		"push ecx;"
		"add edx,0x4;"
		"push edx;"
		"push 0x1;"
		"push eax;"
		"mov eax,esi;"
		"call 0x838800;"
		"jmp L0xABEL_0x008710FB;"
		"L0xABEL_0x00870FF7:;"

		"cmp eax,0x7;"

		"jne L0xABEL_0x0087110F;"
		"cmp dword ptr [ebx+0x14],0x3;"

		"jne L0xABEL_0x0087110F;"

		"mov eax,dword ptr [esi+0x18];"

/* 		"cmp dword ptr [0x011FD23F], 0x1;"
		"je L0xABEL_0x0087110F;"	 */

		"and eax,0xFF000000;" // -> right click input
		"cmp eax,0xFF000000;"

		"je L0xABEL_0x008710A0;"

		"mov ebx,dword ptr [ebx+0x1C];"
		"test cl,bl;"

		"je L0xABEL_0x008710A0;"

		"test bl,0x2;"

		"je L0xABEL_0x008710A0;"

		"mov ebx,dword ptr [edi+0x208];"
		"mov ecx,ebx;"
		"call 0x896590;"
		"test al,al;"
		"jne L0xABEL_0x008710A0;"
		"mov ecx,dword ptr [esp+0x18];"
		"mov edx,dword ptr [ecx+0x18];"
		"mov esi,dword ptr [ebx+0x3FC];"
		"push edx;"
		"call 0x8B5BB0;"
		"test eax,eax;"
		"mov dword ptr [esp+0x10],eax;"
		"je L0xABEL_0x008710D2;"
		"push eax;"
		"call 0x8B43F0;"
		"mov esi,eax;"
		"lea ebx,dword ptr [esp+0x18];"
		"mov dword ptr [esp+0x20],esi;"
		"call 0x7B25F0;"
		"mov esi,dword ptr [esi+0x4];"
		"cmp dword ptr [esp+0x1C],esi;"
		"je L0xABEL_0x008710D2;"
		"L0xABEL_0x00871073:;"
		"lea eax,dword ptr [esp+0x18];"
		"call 0x7B2920;"
		"mov ebx,dword ptr [esp+0x10];"
		"mov esi,eax;"
		"call 0x8B1220;"
		"lea ecx,dword ptr [esp+0x18];"
		"push ecx;"
		"call 0x7F0490;"
		"mov edx,dword ptr [esp+0x20];"
		"mov eax,dword ptr [edx+0x4];"
		"cmp dword ptr [esp+0x1C],eax;"
		"jne L0xABEL_0x00871073;"
		"jmp L0xABEL_0x008710D2;"
		"L0xABEL_0x008710A0:;"
		"cmp dword ptr [edi+0x1A8],0x0;"
		"lea ecx,dword ptr [edi+0x1A8];"
		"je L0xABEL_0x008710D2;"
		"cmp byte ptr [0x10C4840],0x0;"
		"jne L0xABEL_0x008710D2;"
		"cmp dword ptr [edi+0x144],0x6;"
		"mov edx,dword ptr [edi+0x208];"
		"sete al;"
		"push eax;"
		"push edx;"
		"call 0x81FCD0;"
		"add esp,0x8;"
		"L0xABEL_0x008710D2:;"
		"lea esi,dword ptr [esp+0x2C];"
		"call 0x7B25C0;"
		"mov dword ptr [esp+0x164],0xF;"
		"mov eax,dword ptr [edi+0x208];"
		"mov esi,dword ptr [eax+0x400];"
		"mov byte ptr [esi+0x10],0x0;"
		"call 0x8380E0;"
		"L0xABEL_0x008710FB:;"
		"lea eax,dword ptr [esp+0x2C];"
		"mov dword ptr [esp+0x164],0xFFFFFFFF;"
		"call 0x7B2530;"
		"L0xABEL_0x0087110F:;"
		"mov ecx,dword ptr [esp+0x15C];"
		"pop edi;"
		"pop esi;"
		"xor al,al;"
		//"mov dword ptr [0x0],ecx;"
		"pop ebx;"
		"mov esp,ebp;"
		"pop ebp;"
		"ret 0x4;"
	);

}