#ifndef CORVUS_SOURCE_CORVUS_CORE_UUID_H
#define CORVUS_SOURCE_CORVUS_CORE_UUID_H

#include "Corvus/Core/CoreTypes.h"

namespace Corvus
{
    template<typename T>
    struct ::std::hash;

    class FUUID
    {
        friend struct ::std::hash<FUUID>;

    public:
        FUUID();
        FUUID(UInt64 UUID);

        bool operator==(FUUID const &Rhs) const { return m_Value == Rhs.m_Value; }
        bool operator!=(FUUID const &Rhs) const { return m_Value != Rhs.m_Value; }
        bool operator<(FUUID const &Rhs) const { return m_Value < Rhs.m_Value; }
        bool operator<=(FUUID const &Rhs) const { return m_Value <= Rhs.m_Value; }
        bool operator>(FUUID const &Rhs) const { return m_Value > Rhs.m_Value; }
        bool operator>=(FUUID const &Rhs) const { return m_Value >= Rhs.m_Value; }

    private:
        UInt64 m_Value = 0;
    };

} // namespace Corvus

namespace std
{
    template<>
    struct hash<::Corvus::FUUID>
    {
        inline size_t operator()(::Corvus::FUUID const &UUID) const noexcept
        {
            return hash<UInt64>{}(UUID.m_Value);
        }
    };

} // namespace std

#endif // !CORVUS_SOURCE_CORVUS_CORE_UUID_H
