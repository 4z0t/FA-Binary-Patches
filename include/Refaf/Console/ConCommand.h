#pragma once
#include "Refaf/Utility/Base.h"
#include "STD/String.h"
#include "STD/Vector.h"

namespace Refaf
{
    struct ConCommandMap;

    void Con_Printf(const char *format, ...) asm("0x41C990");

    class ConCommand
    {
    protected:
        const char *name;
        const char *description;

    public:
        ConCommand() = delete;
        ConCommand(const char *name, const char *description);

        ConCommand(const ConCommand &) = delete;
        ConCommand(ConCommand &&) = delete;
        ConCommand &operator=(const ConCommand &) = delete;
        ConCommand &operator=(ConCommand &&) = delete;

        ~ConCommand();

        virtual void Handle(const STD::Vector<STD::String> &args) const = 0;
    };

    static_assert(sizeof(ConCommand) == 12);
}