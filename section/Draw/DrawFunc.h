#pragma once
#include "moho.h"

void __fastcall InterlockedExchangeAdd(volatile unsigned *at, unsigned value)
{
    asm("lock xadd [ecx], edx;"
        :
        : "c"(at),
          "d"(value)
        : "cc");
}

// #include <windows.h>
namespace Moho::CPrimBatcher
{
    __stdcall void *FlushBatcher(void *batcher) asm("0x0043A140");
    struct Texture;

    void FromSolidColor(Texture *t, unsigned int color) asm("0x4478C0");
    SHARED
    {
        void *ReleaseTexture(Texture * t);
        void __stdcall SetTexture(void *batcher, Texture *texture);
        void __stdcall SetViewProjMatrix(void *batcher, void *matrix);
    }

    struct Texture
    {

        struct TextureData
        {
            void *vtable;
            int v1;
            int v2;
            int width;
            int height;
            int v5;
        } *data = nullptr;
        struct WeakLock
        {
            void *vtable;
            unsigned use_count_;
            unsigned weak_count_;
            void *px_;

        } *lock = nullptr;

        VALIDATE_SIZE(WeakLock, 16);

        void Lock()
        {
            if (lock)
            {
                InterlockedExchangeAdd(&lock->use_count_, 1);
            }
        }

        void Release() { ReleaseTexture(this); }
    };

} // namespace Moho::CPrimBatcher

struct Vertex
{
    Vector3f v;
    unsigned int color;
    float scaleX;
    float scaleY;
};

SHARED
{
    void _DrawCircle(void *batcher, Vector3f *pos, float radius, float thickness, uint32_t color, Vector3f *orientation);
    void __thiscall CustomDraw(void *_this, void *batcher);
    void _DrawRect(Vector3f * v1, Vector3f * v2, unsigned int color, float thickness, void *batcher, Vector3f *v3, void *heightmap, float f2);
    char *DRAW_WireBox(VMatrix4 * a1, /*CD3DPrimBatcher*/ void *batcher);
    char *DrawLine(Vertex * v1, void *batcher, Vertex *v2);
    char *DrawQuad(Vertex * a1, void *batcher, Vertex *a3, Vertex *a4, Vertex *a5);
    void DrawQuad2(unsigned int color, Vector3f *a2, Vector3f *ecx0, Vector3f *esi0, void *batcher, Vector3f *a6);
}