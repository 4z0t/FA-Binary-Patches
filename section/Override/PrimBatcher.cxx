#include "PrimBatcher.h"

void CheckBatcherMode4(Moho::CD3DPrimBatcher *batcher)
{
    auto num_v = batcher->vertices.Size();
    if (num_v == 0)
        return;

    if (batcher->mode != 4)
    {
        batcher->Flush();
        return;
    }

    if (num_v + 4 > 0x8000)
    {
        batcher->Flush();
        return;
    }

    if (batcher->primitives.Size() + 6 > 0x8000)
    {
        batcher->Flush();
        return;
    }
}

void DrawQuad(Moho::CD3DPrimBatcher *batcher,
              const Moho::Vertex &v1,
              const Moho::Vertex &v2,
              const Moho::Vertex &v3,
              const Moho::Vertex &v4) noexcept(true)
{
    CheckBatcherMode4(batcher);
    batcher->mode = 4;
    // here we know that all vectors have enough space to put things
    auto &vertices = batcher->vertices;
    auto &primitives = batcher->primitives;

    short num_v = vertices.Size();

    Vector2f scale = batcher->p1;
    Vector2f offset = batcher->p2;

    auto f = [&](const Moho::Vertex &v) -> Moho::Vertex
    {
        return {v.pos, v.color, {v.scale.x * scale.x + offset.x, v.scale.y * scale.y + offset.y}};
    };

    *vertices._end = f(v1);
    vertices._end++;
    *vertices._end = f(v2);
    vertices._end++;
    *vertices._end = f(v3);
    vertices._end++;
    *vertices._end = f(v4);
    vertices._end++;

    *primitives._end = num_v;
    primitives._end++;
    *primitives._end = num_v + 1;
    primitives._end++;
    *primitives._end = num_v + 2;
    primitives._end++;
    *primitives._end = num_v + 2;
    primitives._end++;
    *primitives._end = num_v + 3;
    primitives._end++;
    *primitives._end = num_v;
    primitives._end++;
}

SHARED void __stdcall DrawQuad_OVERRIDE(Moho::CD3DPrimBatcher *batcher,
                                        const Moho::Vertex *v1,
                                        const Moho::Vertex *v2,
                                        const Moho::Vertex *v3,
                                        const Moho::Vertex *v4) noexcept(true)

{
    return DrawQuad(batcher, *v1, *v2, *v3, *v4);
}