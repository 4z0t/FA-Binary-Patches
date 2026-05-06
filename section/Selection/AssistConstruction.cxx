#include "AssistConstruction.h"

SHARED void __thiscall CheckCanAssistCon(Moho::SSTICommandIssueData &cmd, incomplete::Command *command_under_mouse)
{
    if (command_under_mouse == nullptr)
        return;

    if (command_under_mouse->commandType == Moho::EUnitCommandType::UNITCOMMAND_BuildMobile)
    {
        cmd.commandType = Moho::EUnitCommandType::UNITCOMMAND_BuildMobile;
        cmd.blueprint = command_under_mouse->bpBuild;
        cmd.target_data.position = command_under_mouse->pos;
        cmd.orientation = {1.0f, 0.0f, 0.0f, 0.0f};
        /**
         * Notes:
         * [x] Any entity below mouse override order, so we have to handle reclaim for example
         * [ ] Have to check that we can build that, otherwise order just cancels
         * [ ] Can stack multiple commands like this. Must filter units that do not queued that order
         */
    }
}

SHARED bool __stdcall CheckReclaimPath(void *command_mode_data, Moho::CWldSession *session)
{
    using namespace Moho;
    uint32_t commandID = GetField<uint32_t>(command_mode_data, 0x58);
    if ((commandID & 0xFF000000) == 0xFF000000)
        return false;

    incomplete::Command *command_under_mouse = GetCommandByID(GetField(session, 0x3FC), commandID);
    if (command_under_mouse == nullptr)
        return false;

    if (command_under_mouse->commandType != EUnitCommandType::UNITCOMMAND_BuildMobile)
        return false;

    SSTICommandIssueData data{EUnitCommandType::UNITCOMMAND_BuildMobile};

    data.target_data = TargetData::Ground(command_under_mouse->pos);
    data.blueprint = command_under_mouse->bpBuild;

    Moho__ISSUE_Command(&session->selectedUnits, &data, 0);

    return true;
}