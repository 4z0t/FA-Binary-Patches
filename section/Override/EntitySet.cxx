#include <algorithm>
#include "EntitySet.h"

uint32_t *__GetEntity(uint32_t *unit)
{
    return unit ? unit + 2 : nullptr;
}

uint32_t __GetIdEntity(uint32_t *entity)
{
    return entity[26];
}

uint32_t __GetId(uint32_t *unit)
{
    return __GetIdEntity(__GetEntity(unit));
}

uint32_t **LinearSearch(uint32_t *unit, uint32_t **begin, uint32_t **end)
{
    uint32_t id = __GetId(unit);
    for (uint32_t **spot = begin; spot != end; ++spot)
    {
        uint32_t s_id = __GetIdEntity(*spot);
        if (s_id >= id)
            return spot;
    }
    return end;
}

uint32_t **BinarySearch(uint32_t *unit, uint32_t **begin, uint32_t **end)
{
    size_t size = end - begin;
    if (size > 0)
    {
        uint32_t id = __GetId(unit);
        do
        {
            size_t middle = size / 2;
            uint32_t m_id = __GetIdEntity(begin[middle]);
            if (m_id == id)
            {
                return begin + middle;
            }

            if (m_id > id)
            {
                size /= 2;
            }
            else
            {
                begin += middle + 1;
                size += -1 - middle;
            }
        } while (size > 0);
    }
    return begin;
}

SHARED Moho::AddResult *__stdcall OVERRIDE_EntitySet_Add(Moho::EntitySet *set, Moho::AddResult *insert_res, uint32_t *unit)
{
    size_t n = set->Size();
    uint32_t **spot = n > 64
                          ? BinarySearch(unit, set->data.begin, set->data.end)
                          : LinearSearch(unit, set->data.begin, set->data.end);

    uint32_t *entity = __GetEntity(unit);
    if (spot == set->data.end || *spot != entity)
    {
        uint32_t index = spot - set->data.begin;
        EntitySetData__InsertAt(&set->data, spot, &entity, &entity + 1);
        insert_res->add_location = &set->data.begin[index];
        insert_res->resized = true;
    }
    else
    {
        insert_res->add_location = spot;
        insert_res->resized = false;
    }

    return insert_res;
}