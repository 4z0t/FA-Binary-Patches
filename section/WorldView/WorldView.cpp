#include "WorldView.h"

int GetEntitiesInView(float *geomcamera, InlinedVector<UserEntity *, 2> *v, Moho::struct_session_res3 *a3, Moho::EEntityType type)
{
    int __result;
    asm(
        "push %[type];"
        "push %[a3];"
        "call 0x005041E0;"
        : "=a"(__result)
        : [geomcamera] "a"(geomcamera), [v] "b"(v), [a3] "g"(a3), [type] "g"(type)
        :);
    return __result;
}