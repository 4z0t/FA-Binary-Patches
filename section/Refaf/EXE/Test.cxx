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
