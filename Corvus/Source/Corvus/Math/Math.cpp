#include "CorvusPCH.h"

#include "Corvus/Math/Math.h"

#include <random>

namespace Corvus
{
    static std::random_device                    s_RandomDevice;
    static std::mt19937_64                       s_RandomGenerator{s_RandomDevice()};
    static std::uniform_int_distribution<UInt64> s_Distribution;

    UInt64 FMath::Random()
    {
        return s_Distribution(s_RandomGenerator);
    }

    UInt64 FMath::Random(UInt64 Min, UInt64 Max)
    {
        std::uniform_int_distribution<UInt64> Distribution(Min, Max);
        return Distribution(s_RandomGenerator);
    }

} // namespace Corvus
