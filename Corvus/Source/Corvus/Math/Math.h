#ifndef CORVUS_SOURCE_CORVUS_MATH_MATH_H
#define CORVUS_SOURCE_CORVUS_MATH_MATH_H

#include <glm/glm.hpp>

namespace Corvus::Math
{
    template<typename T>
    inline constexpr T Min(T V1, T V2)
    {
        return (V1 < V2 ? V1 : V2);
    }

    template<typename T>
    inline constexpr T Max(T V1, T V2)
    {
        return (V1 < V2 ? V2 : V1);
    }

    template<typename T>
    inline constexpr T Clamp(T Value, T Min, T Max)
    {
        return glm::clamp(Value, Min, Max);
    }

    inline constexpr float Radians(float Degrees)
    {
        return glm::radians(Degrees);
    }

    inline constexpr float Degrees(float Radians)
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

} // namespace Corvus::Math

#endif //! CORVUS_SOURCE_CORVUS_MATH_MATH_H