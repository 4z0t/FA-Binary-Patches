#include "Guard.h"

SHARED void __thiscall CheckCanAssistCon(Moho::SSTICommandIssueData &cmd, incomplete::Command *commandUnderMouse)
{
    if (commandUnderMouse == nullptr)
        return;

    if (commandUnderMouse->commandType == Moho::EUnitCommandType::UNITCOMMAND_BuildMobile)
    {
        cmd.commandType = Moho::EUnitCommandType::UNITCOMMAND_BuildMobile;
        cmd.blueprint = commandUnderMouse->bpBuild;
        cmd.target_data.position = commandUnderMouse->pos;
        cmd.orientation = {1.0f, 0.0f, 0.0f, 0.0f};
        /**
         * Notes:
         * Any entity below mouse override order, so we have to handle reclaim for example
         * Have to check that we can build that, otherwise order just cancels
         * Can stack multiple commands like this. Must filter units that do not queued that order
         */
    }
}