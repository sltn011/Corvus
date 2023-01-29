#ifndef CORVUS_SOURCE_CORVUS_CORE_BASE_H
#define CORVUS_SOURCE_CORVUS_CORE_BASE_H

#include "Corvus/Core/CoreTypes.h"
#include "Corvus/Core/PlatformDetection.h"

#include <memory>

#ifdef CORVUS_DEBUG
    #define CORVUS_ENABLE_LOG
    #define CORVUS_ENABLE_ASSERT
    #define CORVUS_ENABLE_PROFILING
    #ifdef CORVUS_PLATFORM_WINDOWS
        #define CORVUS_DEBUG_BREAK() __debugbreak()
    #endif
#elif CORVUS_RELEASE
    #define CORVUS_ENABLE_LOG
    #define CORVUS_ENABLE_ASSERT
    #define CORVUS_DEBUG_BREAK()
#endif

#define CORVUS_BIT(x) (1 << x)

#define CORVUS_EVAL_IF_CONSTEXPR(cond, func, ...) \
    if constexpr (cond)                           \
    {                                             \
        func(__VA_ARGS__);                        \
    }

namespace Corvus
{
    template<typename T>
    using TOwn = std::unique_ptr<T>;

    template<typename T, typename... Args>
    [[nodiscard]] TOwn<T> MakeOwned(Args &&...args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using TRef = std::shared_ptr<T>;

    template<typename T, typename... Args>
    [[nodiscard]] TRef<T> MakeRef(Args &&...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
    inline constexpr std::underlying_type_t<T> EnumRawValue(T EnumValue)
    {
        return static_cast<std::underlying_type_t<T>>(EnumValue);
    }

} // namespace Corvus

#include "Corvus/Core/Assert.h"

#endif //! CORVUS_SOURCE_CORVUS_CORE_BASE_H