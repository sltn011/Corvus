#ifndef CORVUS_SOURCE_CORVUS_MATH_MATH_H
#define CORVUS_SOURCE_CORVUS_MATH_MATH_H

#include "Corvus/Math/Constants.h"

#include <glm/glm.hpp>

namespace Corvus::FMath
{
    template<typename T>
    constexpr T Abs(T Value)
    {
        return glm::abs(Value);
    }

    constexpr bool IsNearlyEqual(float Value1, float Value2, float Epsilon = Constants::SmallNum)
    {
        return Abs(Value1 - Value2) < Epsilon;
    }

    template<typename T>
    constexpr T Min(T Value1, T Value2)
    {
        return glm::min(Value1, Value2);
    }

    template<typename T>
    constexpr T Max(T Value1, T Value2)
    {
        return glm::max(Value1, Value2);
    }

    template<typename T>
    constexpr T Clamp(T Value, T Min, T Max)
    {
        return glm::clamp(Value, Min, Max);
    }

    template<typename T>
    T Floor(float Value)
    {
        return static_cast<T>(glm::floor(Value));
    }

    template<typename T>
    T Ceil(float Value)
    {
        return static_cast<T>(glm::ceil(Value));
    }

    constexpr float Radians(float Degrees)
    {
        return glm::radians(Degrees);
    }

    constexpr float Degrees(float Radians)
    {
        return glm::degrees(Radians);
    }

    inline float Sin(float Degrees)
    {
        return glm::sin(Radians(Degrees));
    }

    inline float Cos(float Degrees)
    {
        return glm::cos(Radians(Degrees));
    }

    inline float Tan(float Degrees)
    {
        return glm::tan(Radians(Degrees));
    }

    inline float Cot(float Degrees)
    {
        return 1.0f / Tan(Degrees);
    }

    inline float ASin(float Value)
    {
        return Degrees(glm::asin(Value));
    }

    inline float ACos(float Value)
    {
        return Degrees(glm::acos(Value));
    }

    inline float ATan(float Value)
    {
        return Degrees(glm::atan(Value));
    }

    inline float ATan2(float Y, float X)
    {
        return Degrees(std::atan2f(Y, X));
    }

    inline float ACot(float Value)
    {
        return ATan(1.0f / Value);
    }

    inline float ACot2(float Y, float X)
    {
        return ATan2(X, Y);
    }

    inline float Sqrt(float Val)
    {
        return glm::sqrt(Val);
    }

    inline float Log2(float Value)
    {
        CORVUS_CORE_ASSERT(Value > 0);
        return log2(Value);
    }

    UInt64 Random();

    UInt64 Random(UInt64 Min, UInt64 Max);

} // namespace Corvus::FMath

#endif //! CORVUS_SOURCE_CORVUS_MATH_MATH_H