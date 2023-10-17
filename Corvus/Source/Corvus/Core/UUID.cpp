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

    CString FUUID::ToString() const
    {
        return std::to_string(m_Value);
    }

} // namespace Corvus
