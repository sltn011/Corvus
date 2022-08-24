#include "CorvusPCH.h"

#include "Corvus/Time/TimePoint.h"

#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{

    FTimePoint::FTimePoint() : m_Value{std::chrono::high_resolution_clock::now()}
    {
    }

    FTimeDelta FTimePoint::operator-(FTimePoint const &Rhs) const
    {
        return FTimeDelta{std::chrono::duration_cast<std::chrono::duration<float>>(m_Value - Rhs.m_Value).count()};
    }

} // namespace Corvus