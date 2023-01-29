#ifndef CORVUS_SOURCE_CORVUS_MATH_CONSTANTS_H
#define CORVUS_SOURCE_CORVUS_MATH_CONSTANTS_H

#include "Corvus/Core/Base.h"

#include <limits>
#include <type_traits>

namespace Corvus::Constants
{

    inline constexpr float Pi = 3.14159265359f;

    inline constexpr float SmallNum     = 0.001f;
    inline constexpr float VerySmallNum = 0.000001f;

    template<typename T, ::std::enable_if_t<::std::is_arithmetic_v<T>, bool> = true>
    constexpr T Max()
    {
        return std::numeric_limits<T>::max();
    }

    template<typename T, ::std::enable_if_t<::std::is_arithmetic_v<T>, bool> = true>
    constexpr T Min()
    {
        return std::numeric_limits<T>::min();
    }

    template<>
    constexpr float Min<float>()
    {
        // std::numeric_limits<float>::min() returns smallest positive value, use lowest instead
        return std::numeric_limits<float>::lowest();
    }

} // namespace Corvus::Constants

#endif //! CORVUS_SOURCE_CORVUS_MATH_CONSTANTS_H