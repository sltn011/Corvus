#ifndef CORVUS_SOURCE_CORVUS_CORE_ASSERT_H
#define CORVUS_SOURCE_CORVUS_CORE_ASSERT_H

#include "Corvus/Core/Base.h"
#include "Corvus/Core/Log.h"

#ifdef CORVUS_ENABLE_ASSERT
    #define CORVUS_ASSERT_IMPL(type, expr, msg, ...)      \
        {                                                 \
            if (!(expr))                                  \
            {                                             \
                CORVUS##type##CRITICAL(msg, __VA_ARGS__); \
                CORVUS_DEBUG_BREAK();                     \
                std::exit(1);                             \
            }                                             \
        }

    #define CORVUS_ASSERT(expr) CORVUS_ASSERT_IMPL(_, expr, "Assertion failed: '{}'", #expr);
    #define CORVUS_ASSERT_FMT(expr, msg, ...) \
        CORVUS_ASSERT_IMPL(_, expr, "Assertion failed: '{}' " msg, #expr, __VA_ARGS__);

    #define CORVUS_CORE_ASSERT(expr) CORVUS_ASSERT_IMPL(_CORE_, expr, "Assertion failed: '{}'", #expr);
    #define CORVUS_CORE_ASSERT_FMT(expr, msg, ...) \
        CORVUS_ASSERT_IMPL(_CORE_, expr, "Assertion failed: '{}' " msg, #expr, __VA_ARGS__);

    #define CORVUS_NO_ENTRY() CORVUS_ASSERT_IMPL(_CRITICAL, false, "NoEntry point reached! {}", "");
    #define CORVUS_NO_ENTRY_FMT(msg, ...) \
        CORVUS_ASSERT_IMPL(_, false, "NoEntry point reached! {}" msg, "", __VA_ARGS__);

    #define CORVUS_CORE_NO_ENTRY() CORVUS_ASSERT_IMPL(_CORE_, false, "NoEntry point reached! {}", "");
    #define CORVUS_CORE_NO_ENTRY_FMT(msg, ...) \
        CORVUS_ASSERT_IMPL(_CORE_, false, "NoEntry point reached! {}" msg, "", __VA_ARGS__);
#else
    #define CORVUS_ASSERT(...)
    #define CORVUS_ASSERT_FMT(...)
    #define CORVUS_CORE_ASSERT(...)
    #define CORVUS_CORE_ASSERT_FMT(...)
    #define CORVUS_NO_ENTRY(...)
    #define CORVUS_NO_ENTRY_FMT(...)
    #define CORVUS_CORE_NO_ENTRY(...)
    #define CORVUS_CORE_NO_ENTRY_FMT(...)
#endif

#endif //! CORVUS_SOURCE_CORVUS_CORE_ASSERT_H