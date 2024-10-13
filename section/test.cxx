#include "CObject.h"
#include "Vector.h"
#include "magic_classes.h"
#include "moho.h"
#include "utility.h"
#include <stdexcept>

struct A
{
    A() { WarningF("Take Resource"); }
    ~A() { WarningF("Free Resource"); }
};

UIRegFunc _{"clang", "q", [](lua_State *L)
            {
                struct A
                {
                    A() { WarningF("Take Resource"); }
                    ~A() { WarningF("Free Resource"); }
                } a;
                try
                {
                    luaL_error(L, "clang error");
                }
                catch (std::runtime_error &e)
                {
                    WarningF("Catch: %s", e.what());
                }
                return 0;
            }};

UIRegFunc __{"test", "q", [](lua_State *l) -> int
             {
                 Vector<A> v{10};
                 return 0;
             }};
