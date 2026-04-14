#include "Refaf/Console/TConVar.h"
#include "Refaf/EXE/Functions.h"
#include "magic_classes.h"

float f_val = 1.0f;
static Refaf::TConVar test_flt{"my_float_var", "", &f_val};

int i_val = 1;
static Refaf::TConVar test_int{"my_int_val", "", &i_val};

UIRegFunc __s{
    "stat", "", [](lua_State *L) -> int
    {
        lua_pushnumber(L, Refaf::InstanceCounter<Refaf::CTask>::GetStatItem()->counter);
        return 1;
    }};

class MyConCommand : public Refaf::ConCommand
{
public:
    MyConCommand() : ConCommand("my_command", "My command") {}
    void Handle(const STD::Vector<STD::String> &args) const override
    {
        for (int i = 0; i < args.size(); i++)
        {
            Refaf::Con_Printf("arg %d: %s", i, args[i].data());
        }
        Refaf::Con_Printf("my_command called");
    }
};

MyConCommand my_command;