#pragma once
#include "../Draw/DrawFunc.h"
#include "GenericStructures.h"
#include "UserUnits.h"

using SPtrTexture = SharedPtr<Moho::CPrimBatcher::Texture>;

struct UserUnitIconsTextures
{
    Moho::UserEntity *entity;
    signed __int32 field_0;
    Vector3f pos;
    SPtrTexture field_14;
    SPtrTexture field_1C;
    SPtrTexture field_24;
    int field_2C[2];
};

VALIDATE_SIZE(UserUnitIconsTextures, 0x34);

struct struct_IconAux // sizeof=0xAC
{
    Vector2f unk;
    Vector2f size;
    Moho::CWldSession *session;
    /*Moho::CD3DPrimBatcher*/ void **batcher;
    /*Moho::GeomCamera3*/ float *cam;
    /*Moho::CWldMap*/ void **map;
    SPtrTexture v8;
    FastVector<void *> genericIconList;
    SPtrTexture pauseRest;
    SPtrTexture stunnedRest;
    FastVector<UserUnitIconsTextures> vec1;
    FastVector<UserUnitIconsTextures> vec2;
    FastVector<UserUnitIconsTextures> vec3;
    FastVector<UserUnitIconsTextures> vec4;
    FastVector<UserUnitIconsTextures> vec5;
    unsigned int selfColor;
    unsigned int neutralColor;
    unsigned int allyColor;
    unsigned int enemyColor;
    unsigned int unidentifiedCOlor;
};

VALIDATE_SIZE(struct_IconAux, 0xAC);

SHARED
{
    void __stdcall ExtendRenderUserUnitIcon(UserUnitIconsTextures * unit_textures,
                                            struct_IconAux * aux,
                                            const Vector2f *pos);
}
