#ifndef CORVUS_SOURCE_CORVUS_CORE_BASE_H
#define CORVUS_SOURCE_CORVUS_CORE_BASE_H

#include "Corvus/Core/CoreTypes.h"
#include "Corvus/Core/PlatformDetection.h"

#include <memory>

#ifdef CORVUS_DEBUG
    #define CORVUS_ENABLE_LOG
    #define CORVUS_ENABLE_ASSERT
    #ifdef CORVUS_PLATFORM_WINDOWS
        #define CORVUS_DEBUG_BREAK() __debugbreak()
    #endif
#else
    #define CORVUS_DEBUG_BREAK()
#endif

#define BIT(x) (1 << x)

namespace Corvus
{
    template<typename T>
    using Own = std::unique_ptr<T>;

    template<typename T, typename... Args>
    [[nodiscard]] Own<T> MakeOwned(Args &&...args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    [[nodiscard]] Ref<T> MakeRef(Args &&...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
} // namespace Corvus

#include "Corvus/Core/Assert.h"

#endif //! CORVUS_SOURCE_CORVUS_CORE_BASE_H