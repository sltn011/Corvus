#ifndef CORVUS_SOURCE_CORVUS_TIME_TIMEPOINT_H
#define CORVUS_SOURCE_CORVUS_TIME_TIMEPOINT_H

#include <chrono>

namespace Corvus
{

    class FTimeDelta;

    class FTimePoint
    {
    public:
        FTimePoint();

        FTimeDelta operator-(FTimePoint const &Rhs) const;

    private:
        std::chrono::high_resolution_clock::time_point m_Value;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_TIME_TIMEPOINT_H
