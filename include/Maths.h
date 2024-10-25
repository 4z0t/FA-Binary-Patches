#pragma once
#include "global.h"
struct VMatrix4
{
    float data[16];
};
VALIDATE_SIZE(VMatrix4, 0x40);

struct Vector2f
{ // 0x8 bytes
    float x, y;
};

struct Vector3f
{ // 0xC bytes
    float x, y, z;

    Vector3f operator+(const Vector3f &other) const
    {
        return {x + other.x, y + other.y, z + other.z};
    }
};

struct Vector4f
{ // 0x10 bytes
    float x, y, z, w;
};