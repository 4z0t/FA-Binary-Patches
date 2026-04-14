#pragma once
#include "Refaf/Console/ConCommand.h"
#include "Refaf/EXE/Functions.h"

namespace Refaf
{
    ConCommand::ConCommand(const char *name, const char *description) : name{name}, description{description}
    {
        Con_RegisterConCommand(Con_GetMap(), this);
    }

    ConCommand::~ConCommand()
    {
        Con_UnregisterCommand(Con_GetMap(), this);
    }
};