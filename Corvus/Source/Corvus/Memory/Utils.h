#ifndef CORVUS_SOURCE_CORVUS_MEMORY_UTILS_H
#define CORVUS_SOURCE_CORVUS_MEMORY_UTILS_H

#include "Corvus/Core/Base.h"

namespace Corvus
{

    enum class EContainerGrowthCoeff : UInt8
    {
        Double,
    };

    float GetContainerGrowthValue(EContainerGrowthCoeff Coeff);

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_MEMORY_UTILS_H
