#ifndef CORVUS_SOURCE_CORVUS_TIME_TIMEDELTA_H
#define CORVUS_SOURCE_CORVUS_TIME_TIMEDELTA_H

namespace Corvus
{

    class TimeDelta
    {
    public:
        TimeDelta(float Seconds);

        float Seconds() const;
        float MilliSeconds() const;
        float MicroSeconds() const;

        float Minutes() const;

    private:
        float m_DeltaSeconds;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_TIME_TIMEDELTA_H
