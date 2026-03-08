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
    Vector2f *__attribute__((regparm(3))) ComputeUnitPosOOnScreen(Vector2f * result, const UserUnitIconsTextures *t, const struct_IconAux *aux);

    int GetEntitiesInView(float *geomcamera, InlinedVector<UserEntity *, 2> *v, Moho::struct_session_res3 *a3, Moho::EEntityType type);
}