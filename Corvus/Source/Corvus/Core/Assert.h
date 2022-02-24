#ifndef CORVUS_SOURCE_CORVUS_CORE_ASSERT_H
#define CORVUS_SOURCE_CORVUS_CORE_ASSERT_H

#include "Corvus/Core/Base.h"
#include "Corvus/Core/Log.h"

#ifdef CORVUS_ENABLE_ASSERT
    #define CORVUS_ASSERT_IMPL(type, expr, msg, ...) {if(!(expr)) {CORVUS##type##ERROR(msg, __VA_ARGS__); CORVUS_DEBUG_BREAK();}}

    #define CORVUS_ASSERT(expr, msg, ...) CORVUS_ASSERT_IMPL(_, expr, "Assertion failed: '{0}'! "##msg, #expr, __VA_ARGS__);
    #define CORVUS_CORE_ASSERT(expr, msg, ...) CORVUS_ASSERT_IMPL(_CORE_, expr, "Assertion failed: '{0}'! "##msg, #expr, __VA_ARGS__);
#else
    #define CORVUS_ASSERT(...)
    #define CORVUS_CORE_ASSERT(...)
#endif

#endif //!CORVUS_SOURCE_CORVUS_CORE_ASSERT_H