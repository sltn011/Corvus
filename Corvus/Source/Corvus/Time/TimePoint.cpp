#include "CorvusPCH.h"

#include "Corvus/Time/TimePoint.h"

#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{

    TimePoint::TimePoint() : m_Value{std::chrono::high_resolution_clock::now()}
    {
    }

    TimeDelta TimePoint::operator-(TimePoint const &Rhs) const
    {
        return TimeDelta{std::chrono::duration_cast<std::chrono::duration<float>>(m_Value - Rhs.m_Value).count()};
    }

} // namespace Corvus