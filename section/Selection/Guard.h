#pragma once

#include "Selection.h"
#include "Iterators.h"
#include <exception>

#include "../IssueCommand/IssueCommand.h"

struct CFormation
{
    int v0;
    int vec;
    int v2;
    /*  Moho::CAiFormationInstance*/ void *curInstance;
    bool ready;
    // padding byte
    // padding byte
    // padding byte
    int type;
    Vector3f start;
    Vector3f finish;
    Vector3f mousePos;
    int bestFormation;
    int travelFormation;
    int numFormationScripts;
    Quaternion direction;
    float v22;
    float timeLeft;
    float lastUpdate;
};

void __stdcall Moho__CFormation__ChooseFormation(CFormation *formation, Vector3f *pos, Moho::UserUnitMap *units, bool shift) asm("0x008384C0");

SHARED
{
    void Moho__ISSUE_FactoryCommand(Moho::UserUnitMap * units, Moho::SSTICommandIssueData * data, int clear);
    void Moho__ISSUE_Command(Moho::UserUnitMap * units, Moho::SSTICommandIssueData * a2, int clear);

    void __thiscall HandleGroundAssist(void *__this);

    void Moho__CFormation__Reset(CFormation * formation);
}
