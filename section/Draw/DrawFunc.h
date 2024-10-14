#pragma once
#include "moho.h"

bool InterlockedExchangeAdd(volatile unsigned *addr, unsigned value)
{
    bool _result;
    asm(
        "lock xadd [eax], edx;"
        "setnz al;"
        : "=a"(_result)
        : "a"(addr),
          "d"(value)
        :);
    return _result;
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
            struct vtable_counted_base
            {
                void(__thiscall *dtr)(WeakLock *);
                void(__thiscall *dispose)(WeakLock *);
                void(__thiscall *destroy)(WeakLock *);
                void *(__thiscall *get_deleter)(WeakLock *);
            } *vtable;
            unsigned use_count_;
            unsigned weak_count_;
            void *px_;

        } *lock = nullptr;

        VALIDATE_SIZE(WeakLock, 16);
        Texture &operator=(const Texture &other)
        {
            if (lock)
                Release();
            data = other.data;
            lock = other.lock;
            Lock();
        }

        Texture &operator=(Texture &&other)
        {
            if (lock)
                Release();
            data = other.data;
            lock = other.lock;
            other.data = nullptr;
            other.lock = nullptr;
        }

        Texture() : data{nullptr}, lock{nullptr}
        {
        }

        Texture(Texture &&other)
        {
            data = other.data;
            lock = other.lock;
            other.data = nullptr;
            other.lock = nullptr;
        }

        void Lock()
        {
            if (lock)
            {
                InterlockedExchangeAdd(&lock->use_count_, 1);
            }
        }

        void Release()
        {
            if (!lock)
                return;
            if (!InterlockedExchangeAdd(&lock->use_count_, -1))
            {
                lock->vtable->dispose(lock);
                if (!InterlockedExchangeAdd(&lock->weak_count_, -1))
                {
                    lock->vtable->destroy(lock);
                }
            }
            // ReleaseTexture(this);
        }
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