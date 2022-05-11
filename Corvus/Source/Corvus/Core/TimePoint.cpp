#include "CorvusPCH.h"
#include "Corvus/Core/TimePoint.h"

namespace Corvus
{

    TimePoint::TimePoint()
        : m_Value{ std::chrono::high_resolution_clock::now() }
    {
    }

    // Duration in seconds between two timepoints
    float TimePoint::operator-(TimePoint const &Rhs) const
    {
        return std::chrono::duration_cast<std::chrono::duration<float>>(m_Value - Rhs.m_Value).count();
    }

}