#ifndef CORVUS_SOURCE_CORVUS_CORE_BASE_H
#define CORVUS_SOURCE_CORVUS_CORE_BASE_H

#include "Corvus/Core/PlatformDetection.h"
#include "Corvus/Core/CoreTypes.h"

#ifdef CORVUS_DEBUG
    #define CORVUS_ENABLE_ASSERT
    #ifdef CORVUS_PLATFORM_WINDOWS
        #define CORVUS_DEBUG_BREAK() __debugbreak()
    #endif
#else
    #define CORVUS_DEBUG_BREAK()
#endif

#define BIT(x) (1 << x)

#define STR(x) L##x

#include "Corvus/Core/Assert.h"

#endif //!CORVUS_SOURCE_CORVUS_CORE_BASE_H