#ifndef CORVUS_SOURCE_CORVUS_CORE_LOG_H
#define CORVUS_SOURCE_CORVUS_CORE_LOG_H

#include <memory>
#include "spdlog/spdlog.h"

namespace Corvus {

    class Log
    {
    public:

        Log();
        ~Log();

        static void Init();

        static std::shared_ptr<spdlog::logger> &GetEngineLogger() { return s_EngineLogger; }
        static std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

    protected:

        static std::shared_ptr<spdlog::logger> s_EngineLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

}

#define CORVUS_CORE_TRACE(...)      ::Corvus::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define CORVUS_CORE_INFO(...)       ::Corvus::Log::GetEngineLogger()->info(__VA_ARGS__)
#define CORVUS_CORE_WARN(...)       ::Corvus::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define CORVUS_CORE_ERROR(...)      ::Corvus::Log::GetEngineLogger()->error(__VA_ARGS__)
#define CORVUS_CORE_CRITICAL(...)   ::Corvus::Log::GetEngineLogger()->critical(__VA_ARGS__)

#define CORVUS_TRACE(...)           ::Corvus::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CORVUS_INFO(...)            ::Corvus::Log::GetClientLogger()->info(__VA_ARGS__)
#define CORVUS_WARN(...)            ::Corvus::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CORVUS_ERROR(...)           ::Corvus::Log::GetClientLogger()->error(__VA_ARGS__)
#define CORVUS_CRITICAL(...)        ::Corvus::Log::GetClientLogger()->critical(__VA_ARGS__)

#endif //!CORVUS_SOURCE_CORVUS_CORE_LOG_H