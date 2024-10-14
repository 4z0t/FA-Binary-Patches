#pragma once
#include "GenericStructures.h"
#include "Maths.h"
#include "SharedPtr.h"
#include "global.h"

namespace Moho
{
    struct Vertex
    {
        Vector3f pos;
        uint32_t color;
        Vector2f scale;
    };
    struct CD3DPrimBatcher // sizeof=0x124
    {
        /*Moho::CD3DTextureBatcher */ void *texBatcher;
        /*Moho::CD3DVertexSheet*/ void *vertexSheets[3];
        int curVertexSheet;
        /* Moho::CD3DIndexSheet*/ void *indexSheet;
        FastVector<Vertex> vertices;
        FastVector<short> primitives;
        int mode;
        SharedPtr<void *> tex1;
        SharedPtr<void *> batchTexture;
        Vector2f p2;
        Vector2f p1;
        VMatrix4 viewMatrix;
        VMatrix4 projectionMatrix;
        VMatrix4 composite;
        bool resetComposite;
        bool rebuildComposite;
        // padding byte
        // padding byte
        float alphaMultiplier;

        void __stdcall Flush() asm("0x0043A140");
    };
}
VALIDATE_SIZE(Moho::CD3DPrimBatcher, 0x124);