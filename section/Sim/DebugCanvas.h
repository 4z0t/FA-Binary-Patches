#pragma once
#include "moho.h"

/*Moho::CDebugCanvas*/ void *__thiscall Moho__Sim__GetDebugCanvas(/*Moho::Sim*/ void *sim) asm("0x746720");

SHARED
{
    void Moho__CDebugCanvas__AddWireCoords(Vector3f * pos, /* Moho::CDebugCanvas */ void *canvas, Quaternion *a3, float a4);
}
