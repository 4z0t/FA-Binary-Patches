#pragma once

#include <stdexcept>
#include <exception>
#include "global.h"

// struct __std_exception_data
// {
//     char const *_What;
//     bool _DoFree;
// };

struct XException
{
    int pad[10]; // std::runtime_error
    int stack[32];
    int count;
    int pad2[7]; // std::string
};

XException *__stdcall Moho__XException__XException(XException *data, const char *msg) asm("0x004052A0");
XException *__thiscall Moho__XException__Dtor(XException *_this, char a2) asm("0x00405370");
unsigned int __cdecl Moho__PLAT_GetCallStack(unsigned int size, void *result) asm("0x004A22B0");
string *__cdecl Moho__PLAT_FormatCallstack(string *result, int offset, int count, void *addresses) asm("0x004A26E0");

void MohoError(const char *msg);
string Moho__GetCallStack(int offset = 0);
