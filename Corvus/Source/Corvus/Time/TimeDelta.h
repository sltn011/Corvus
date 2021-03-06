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

    protected:

        float m_DeltaSeconds;
    };

}

#endif // !CORVUS_SOURCE_CORVUS_TIME_TIMEDELTA_H
