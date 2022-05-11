#ifndef CORVUS_SOURCE_CORVUS_CORE_TIMEPOINT_H
#define CORVUS_SOURCE_CORVUS_CORE_TIMEPOINT_H

#include <chrono>

namespace Corvus
{

    class TimePoint
    {
    public:

        TimePoint();

        // Duration in seconds between two timepoints
        float operator-(TimePoint const &Rhs) const;

    protected:

        std::chrono::high_resolution_clock::time_point m_Value;
    };

}

#endif // !CORVUS_SOURCE_CORVUS_CORE_TIMEPOINT_H
