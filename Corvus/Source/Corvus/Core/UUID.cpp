#include "CorvusPCH.h"

#include "Corvus/Core/UUID.h"

namespace Corvus
{

    FUUID::FUUID() : m_Value{FMath::Random()}
    {
    }

    FUUID::FUUID(UInt64 UUID) : m_Value{UUID}
    {
    }

} // namespace Corvus
