#include "Refaf/InstanceCounter.h"
#include "Refaf/UserUnit.h"
#include "Refaf/Entity.h"
#include "Refaf/CTask.h"

namespace Refaf
{
    template <>
    StatItem *InstanceCounter<CTask>::GetStatItem() asm("0x0040AB50");

} // namespace Refaf
