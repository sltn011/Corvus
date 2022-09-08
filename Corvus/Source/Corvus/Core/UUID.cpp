#include "CorvusPCH.h"

#include "Corvus/Core/UUID.h"

#include <random>

namespace Corvus
{
    static std::random_device                    s_RandomDevice;
    static std::mt19937_64                       s_RandomGenerator{s_RandomDevice()};
    static std::uniform_int_distribution<UInt64> s_Distribution;

    FUUID::FUUID() : m_Value{s_Distribution(s_RandomGenerator)}
    {
    }

    FUUID::FUUID(UInt64 UUID) : m_Value{UUID}
    {
    }

} // namespace Corvus
