#include "CorvusPCH.h"

#include "Corvus/Memory/Utils.h"

namespace Corvus
{

    float GetContainerGrowthValue(EContainerGrowthCoeff const Coeff)
    {
        switch (Coeff)
        {
        case EContainerGrowthCoeff::Double:
            return 2.0f;

        default:
            return 1.0f;
        }
    }

} // namespace Corvus
