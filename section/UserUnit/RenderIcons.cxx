#include "RenderIcons.h"

void __stdcall ExtendRenderUserUnitIcon(UserUnitIconsTextures *unit_textures,
                                        struct_IconAux *aux,
                                        const Vector2f *pos)
{
    using namespace Moho;
    UserEntity *entity = unit_textures->entity;
    UserUnit *unit = entity->IsUserUnit();
    if (!unit)
        return;

    SPtrTexture *texture = Offset<SPtrTexture *>(unit, 1000);
    if (!texture->data)
    {
        return;
    }

    void *batcher = aux->batcher;
    CPrimBatcher::SetTexture(batcher, texture);

    Vector3f t_r;
    Vector3f t_l;
    Vector3f b_l;
    Vector3f b_r;

    float half_width = texture->data->width / 2;
    float half_height = texture->data->height / 2;
    t_r.x = pos->x + half_width;
    t_r.y = pos->y - half_height;
    t_r.z = 0.0;
    b_r.x = pos->x + half_width;
    b_r.y = pos->y + half_height;
    b_r.z = 0.0;
    b_l.x = pos->x - half_width;
    b_l.y = pos->y + half_height;
    b_l.z = 0.0;
    t_l.x = pos->x - half_width;
    t_l.y = pos->y - half_height;
    t_l.z = 0.0;
    DrawQuad2(0xFFFFFFFF, &b_r, &t_r, &b_l, batcher, &t_l);
}