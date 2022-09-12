#ifndef CORVUS_SOURCE_CORVUS_CORE_LOG_H
#define CORVUS_SOURCE_CORVUS_CORE_LOG_H

#include "Corvus/Core/Base.h"

#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#pragma warning(pop)

namespace Corvus
{

    class CLog
    {
    public:
        CLog();
        ~CLog();

        static void Init();

        static TRef<spdlog::logger> &GetEngineLogger() { return s_EngineLogger; }
        static TRef<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

        static TRef<spdlog::logger> &GetDumpLogger() { return s_DumpLogger; }

    private:
        static TRef<spdlog::logger> s_EngineLogger;
        static TRef<spdlog::logger> s_ClientLogger;
        static TRef<spdlog::logger> s_DumpLogger;
    };

} // namespace Corvus

#ifdef CORVUS_ENABLE_LOG
    #define CORVUS_CORE_TRACE(...)    ::Corvus::CLog::GetEngineLogger()->trace(__VA_ARGS__)
    #define CORVUS_CORE_INFO(...)     ::Corvus::CLog::GetEngineLogger()->info(__VA_ARGS__)
    #define CORVUS_CORE_WARN(...)     ::Corvus::CLog::GetEngineLogger()->warn(__VA_ARGS__)
    #define CORVUS_CORE_ERROR(...)    ::Corvus::CLog::GetEngineLogger()->error(__VA_ARGS__)
    #define CORVUS_CORE_CRITICAL(...) ::Corvus::CLog::GetEngineLogger()->critical(__VA_ARGS__)

    #define CORVUS_TRACE(...)    ::Corvus::CLog::GetClientLogger()->trace(__VA_ARGS__)
    #define CORVUS_INFO(...)     ::Corvus::CLog::GetClientLogger()->info(__VA_ARGS__)
    #define CORVUS_WARN(...)     ::Corvus::CLog::GetClientLogger()->warn(__VA_ARGS__)
    #define CORVUS_ERROR(...)    ::Corvus::CLog::GetClientLogger()->error(__VA_ARGS__)
    #define CORVUS_CRITICAL(...) ::Corvus::CLog::GetClientLogger()->critical(__VA_ARGS__)

    #define CORVUS_DUMP(...) ::Corvus::CLog::GetDumpLogger()->critical(__VA_ARGS__)
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

    #define CORVUS_DUMP(...)
#endif
#endif //! CORVUS_SOURCE_CORVUS_CORE_LOG_H