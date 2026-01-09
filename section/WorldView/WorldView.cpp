#include "WorldView.h"

SHARED void Project(float *camera, const Vector3f *v, Vector2f *result)
{
    asm(
        "call 0x471080;"
        :
        : "a"(result),
          "d"(v),
          "c"(camera)
        //: "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7"
    );
}
