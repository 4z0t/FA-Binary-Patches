#include "moho.h"

struct struct_Resources // sizeof=0x8
{
    float ENERGY;
    float MASS;
};

struct struct_ConsumptionData
{
    struct_ConsumptionData *next;
    struct_ConsumptionData *prev;
    struct_Resources perSecond;
    struct_Resources addWhenSetOff;
};

struct struct_UnitData
{
    int dword0;
    int dword4;
    bool autoMode;
    bool autoSurfaceMode;
    bool isBusy;
    bool byteB;
    float fuelRatio;
    float shieldRatio;
    int stunTicks;
    bool isPaused;
    bool isValidTarget;
    bool byte1A;
    bool byte1B;
    float dword1C;
    /*Moho::EFireState*/ int fireState;
    float workProgress;
    int tacticalSiloBuildCount;
    int nukeSiloBuildCount;
    int tacticalSiloStorageCount;
    int nukeSiloStorageCount;
    int tacticalSiloMaxStorageCount;
    int nukeSiloMaxStorageCount;
    int dword40;
    string customName;
    float energyProduced;
    float massProduced;
    float energySpent;
    float massSpent;
    float energyMaintainenceCost;
    float massMaintainanceCost;
    int focusUnit;
    int dword7C;
    int dword80;
    int dword84;
    int dword88;
    int dword8C;
    int dword90;
    Vector3f vec3;
    int dwordA0;
    int dwordA4;
    int gapA8;
    bool v0;
    bool v1;
    bool v2;
    bool v3;
    bool gapB0[24];
    int dwordC8;
    int dwordCC;
    int dwordD0;
    int dwordD4;
    int gapD8;
    int gapDC;
    int gapE0;
    int gapE4;
    int gapE8;
    float gapEC;
    int gapF0;
    int gapF4;
    int gapF8;
    bool gapFC[164];
    /*Moho::UnitAttributes*/ char attributes[0x70];
    unsigned int scriptbits;
    bool gap214[4];
    int unitStates1;
    int unitStates2;
    /*Moho::CUnitMotion*/ void *unitMotion;
};
VALIDATE_SIZE(struct_UnitData, 0x224);

struct Moho_Unit
{
    void *vtable;
    void *chain;
    char entity[0x270];
    /*struct_StatItemRRef*/ char statRref[0x10];
    struct_UnitData unitData;
};

struct CAiSiloBuildImpl // sizeof=0x4C
{
    void *vtable;
    Moho_Unit *unit;
    int vec[3];
    int v5;
    int v6;
    int v7;
    int v8;
    int *v9;
    int v10;
    struct_ConsumptionData *cons_data;
    signed __int32 state;
    float energySegmentCost;
    float massSegmentCost;
    float segmentEnergySpent;
    float segmentMassSpent;
    float segments;
    float curSegments;
};

SHARED void __stdcall CheckSiloProgress(CAiSiloBuildImpl *silo_build, struct_Resources *res)
{
    // LogF("%s", "hello");
    struct_ConsumptionData *cons_data = silo_build->cons_data;

    float energy_ratio = cons_data->addWhenSetOff.ENERGY / silo_build->energySegmentCost;
    float mass_ratio = cons_data->addWhenSetOff.MASS / silo_build->massSegmentCost;
    if (mass_ratio >= 1 && energy_ratio >= 1)
    {
        *res = cons_data->addWhenSetOff;
        cons_data->addWhenSetOff.ENERGY = 0;
        cons_data->addWhenSetOff.MASS = 0;
        auto *unit = silo_build->unit;
        unit->unitData.energyMaintainenceCost = silo_build->energySegmentCost;
        unit->unitData.massMaintainanceCost = silo_build->massSegmentCost;
        float *v22 = (float *)silo_build->unit;
        v22[188] += res->ENERGY;
        v22[189] += res->MASS;
        silo_build->curSegments += 1;
        silo_build->unit->unitData.workProgress = silo_build->curSegments / silo_build->segments;
    }
    else
    {
        float available_ratio = (mass_ratio > energy_ratio ? energy_ratio : mass_ratio);

        *res = cons_data->addWhenSetOff;
        cons_data->addWhenSetOff.ENERGY = 0;
        cons_data->addWhenSetOff.MASS = 0;

        auto *unit = silo_build->unit;
        unit->unitData.energyMaintainenceCost = res->ENERGY;
        unit->unitData.massMaintainanceCost = res->MASS;
        float *v22 = (float *)silo_build->unit;
        v22[188] += res->ENERGY;
        v22[189] += res->MASS;
        LogF("%.2f/%.2f", silo_build->curSegments, silo_build->segments);
        silo_build->curSegments += available_ratio;
        silo_build->unit->unitData.workProgress = silo_build->curSegments / silo_build->segments;
    }

    if (silo_build->curSegments >= silo_build->segments)
    {
        silo_build->state = 3;
    }
}

SHARED void __thiscall AddSiloEco(CAiSiloBuildImpl *silo_build, struct_Resources *econ)
{
    float result;
    float v3;
    float segmentMassSpent;
    float v5;
    float v6;
    int unit;
    float v8;

    if (!silo_build->state)
        return;

    silo_build->segmentEnergySpent = silo_build->segmentEnergySpent + econ->ENERGY;
    silo_build->segmentMassSpent = silo_build->segmentMassSpent + econ->MASS;
    for (result = silo_build->segmentEnergySpent; result >= silo_build->energySegmentCost; result = silo_build->segmentEnergySpent)
    {
        result = silo_build->segmentMassSpent;
        if (result < silo_build->massSegmentCost)
            break;
        v3 = 0.0;
        if (silo_build->segmentEnergySpent - silo_build->energySegmentCost > 0.0)
            v3 = silo_build->segmentEnergySpent - silo_build->energySegmentCost;
        segmentMassSpent = silo_build->segmentMassSpent;
        silo_build->segmentEnergySpent = v3;
        v5 = segmentMassSpent - silo_build->massSegmentCost;
        v6 = 0.0;
        if (v5 > 0.0)
            v6 = v5;
        silo_build->segmentMassSpent = v6;
        unit = (int)silo_build->unit;
        v8 = silo_build->energySegmentCost + *(float *)(unit + 752);
        unit += 0x2F0;
        *(float *)unit = v8;
        *(float *)(unit + 4) = silo_build->massSegmentCost + *(float *)(unit + 4);
        silo_build->unit->unitData.workProgress = (float)silo_build->curSegments++ / silo_build->segments;
        if (silo_build->curSegments >= silo_build->segments)
            silo_build->state = 3;
    }
}