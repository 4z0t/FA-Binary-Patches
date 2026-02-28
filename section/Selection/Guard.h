#pragma once

#include "Selection.h"
#include "Iterators.h"
#include <exception>

#include "../IssueCommand/IssueCommand.h"

SHARED
{
    void Moho__ISSUE_FactoryCommand(Moho::UserUnitMap * units, Moho::SSTICommandIssueData * data, int clear);
    void Moho__ISSUE_Command(Moho::UserUnitMap * units, Moho::SSTICommandIssueData * a2, int clear);

    void __thiscall HandleGroundAssist(void *__this);
}
