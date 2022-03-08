#ifndef CORVUS_SOURCE_CORVUS_CORE_LOG_H
#define CORVUS_SOURCE_CORVUS_CORE_LOG_H

#include "Corvus/Core/Base.h"

#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#pragma warning(pop)

namespace Corvus 
{

    class Log
    {
    public:

        Log();
        ~Log();

        static void Init();

        static Ref<spdlog::logger> &GetEngineLogger() { return s_EngineLogger; }
        static Ref<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

    protected:

        static Ref<spdlog::logger> s_EngineLogger;
        static Ref<spdlog::logger> s_ClientLogger;
    };

}

#ifdef CORVUS_ENABLE_LOG
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
#endif //!CORVUS_SOURCE_CORVUS_CORE_LOG_H