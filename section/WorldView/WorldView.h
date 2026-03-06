#pragma once
#include "CObject.h"
#include "magic_classes.h"
#include "moho.h"
#include "utility.h"
#include "../UserUnit/RenderIcons.h"

SHARED
{
    Vector2f *__thiscall Moho__CameraImpl__Project(/*Moho::CameraImpl*/ const void *__this, Vector2f *result, Vector3f *pos);
    Vector2f *__thiscall Moho__GeomCamera__Project(const float *cam, Vector2f *result, Vector3f *pos);
    Vector2f *__fastcall ComputeUnitPosOOnScreen(const UserUnitIconsTextures *t, const struct_IconAux *aux, Vector2f *result);
}