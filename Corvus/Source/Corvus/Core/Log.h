#ifndef CORVUS_SOURCE_CORVUS_CORE_LOG_H
#define CORVUS_SOURCE_CORVUS_CORE_LOG_H

#include "Corvus/Core/Base.h"

#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#pragma warning(pop)

namespace Corvus
{

    class СLog
    {
    public:
        СLog();
        ~СLog();

        static void Init();

        static TRef<spdlog::logger> &GetEngineLogger() { return s_EngineLogger; }
        static TRef<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

    private:
        static TRef<spdlog::logger> s_EngineLogger;
        static TRef<spdlog::logger> s_ClientLogger;
    };

} // namespace Corvus

#ifdef CORVUS_ENABLE_LOG
    #define CORVUS_CORE_TRACE(...)    ::Corvus::СLog::GetEngineLogger()->trace(__VA_ARGS__)
    #define CORVUS_CORE_INFO(...)     ::Corvus::СLog::GetEngineLogger()->info(__VA_ARGS__)
    #define CORVUS_CORE_WARN(...)     ::Corvus::СLog::GetEngineLogger()->warn(__VA_ARGS__)
    #define CORVUS_CORE_ERROR(...)    ::Corvus::СLog::GetEngineLogger()->error(__VA_ARGS__)
    #define CORVUS_CORE_CRITICAL(...) ::Corvus::СLog::GetEngineLogger()->critical(__VA_ARGS__)

    #define CORVUS_TRACE(...)    ::Corvus::СLog::GetClientLogger()->trace(__VA_ARGS__)
    #define CORVUS_INFO(...)     ::Corvus::СLog::GetClientLogger()->info(__VA_ARGS__)
    #define CORVUS_WARN(...)     ::Corvus::СLog::GetClientLogger()->warn(__VA_ARGS__)
    #define CORVUS_ERROR(...)    ::Corvus::СLog::GetClientLogger()->error(__VA_ARGS__)
    #define CORVUS_CRITICAL(...) ::Corvus::СLog::GetClientLogger()->critical(__VA_ARGS__)
#else
    #define CORVUS_CORE_TRACE(...)
    #define CORVUS_CORE_INFO(...)
    #define CORVUS_CORE_WARN(...)
    #define CORVUS_CORE_ERROR(...)
    #define CORVUS_CORE_CRITICAL(...)

    #define CORVUS_TRACE(...)
    #define CORVUS_INFO(...)
    #define CORVUS_WARN(...)
    #define CORVUS_ERROR(...)
    #define CORVUS_CRITICAL(...)
#endif
#endif //! CORVUS_SOURCE_CORVUS_CORE_LOG_H