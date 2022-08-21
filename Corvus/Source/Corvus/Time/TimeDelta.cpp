#include "CorvusPCH.h"

#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{

    TimeDelta::TimeDelta(float const Seconds) : m_DeltaSeconds{Seconds}
    {
    }

    float TimeDelta::Seconds() const
    {
        return m_DeltaSeconds;
    }

    float TimeDelta::MilliSeconds() const
    {
        return m_DeltaSeconds * 1e3f;
    }

    float TimeDelta::MicroSeconds() const
    {
        return m_DeltaSeconds * 1e6f;
    }

    float TimeDelta::Minutes() const
    {
        return m_DeltaSeconds / 60.0f;
    }

} // namespace Corvus