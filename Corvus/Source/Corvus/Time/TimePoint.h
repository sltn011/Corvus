#ifndef CORVUS_SOURCE_CORVUS_TIME_TIMEPOINT_H
#define CORVUS_SOURCE_CORVUS_TIME_TIMEPOINT_H

#include <chrono>

namespace Corvus
{

    class TimeDelta;

    class TimePoint
    {
    public:

        TimePoint();

        TimeDelta operator-(TimePoint const &Rhs) const;

    protected:

        std::chrono::high_resolution_clock::time_point m_Value;
    };

}

#endif // !CORVUS_SOURCE_CORVUS_TIME_TIMEPOINT_H
