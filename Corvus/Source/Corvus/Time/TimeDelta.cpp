#include "CorvusPCH.h"

#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{

    FTimeDelta::FTimeDelta(float const Seconds) : m_DeltaSeconds{Seconds}
    {
    }

    float FTimeDelta::Seconds() const
    {
        return m_DeltaSeconds;
    }

    float FTimeDelta::MilliSeconds() const
    {
        return m_DeltaSeconds * 1e3f;
    }

    float FTimeDelta::MicroSeconds() const
    {
        return m_DeltaSeconds * 1e6f;
    }

    float FTimeDelta::Minutes() const
    {
        return m_DeltaSeconds / 60.0f;
    }

} // namespace Corvus