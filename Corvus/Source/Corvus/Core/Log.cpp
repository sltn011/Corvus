#include "CorvusPCH.h"

#include "Corvus/Core/Log.h"

#pragma warning(push, 0)
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma warning(pop)

namespace Corvus
{

    TRef<spdlog::logger> CLog::s_EngineLogger;
    TRef<spdlog::logger> CLog::s_ClientLogger;
    TRef<spdlog::logger> CLog::s_DumpLogger;

    CLog::CLog()
    {
    }

    CLog::~CLog()
    {
    }

    void CLog::Init()
    {
        // Core and User loggers
        {
            std::vector<spdlog::sink_ptr> LogSinks;
            LogSinks.push_back(MakeRef<spdlog::sinks::stdout_color_sink_mt>());
            LogSinks.push_back(MakeRef<spdlog::sinks::basic_file_sink_mt>("Corvus.log", true));

            for (spdlog::sink_ptr &Sink : LogSinks)
            {
                Sink->set_pattern("%^[%D %T][%l] %n: %v%$"
                ); // COLOR([MM/DD/YY HH:MM:SS][loglevel] LogName: Message)
            }

            s_EngineLogger = MakeRef<spdlog::logger>("CORVUS", LogSinks.begin(), LogSinks.end());
            s_EngineLogger->set_level(spdlog::level::trace);
            s_EngineLogger->flush_on(spdlog::level::trace);
            spdlog::register_logger(s_EngineLogger);

            s_ClientLogger = MakeRef<spdlog::logger>("APP", LogSinks.begin(), LogSinks.end());
            s_ClientLogger->set_level(spdlog::level::trace);
            s_ClientLogger->flush_on(spdlog::level::trace);
            spdlog::register_logger(s_ClientLogger);
        }

        // Dump logger
        {
            std::vector<spdlog::sink_ptr> DumpLogSinks;
            DumpLogSinks.push_back(MakeRef<spdlog::sinks::basic_file_sink_st>("CorvusDump.log", true));

            s_DumpLogger = MakeRef<spdlog::logger>("DUMP", DumpLogSinks.begin(), DumpLogSinks.end());
            TOwn<spdlog::pattern_formatter> Formatter = MakeOwned<spdlog::pattern_formatter>(
                "%v", spdlog::pattern_time_type::local, CString{""}
            ); // No \n
            s_DumpLogger->set_formatter(std::move(Formatter));
            s_EngineLogger->set_level(spdlog::level::trace);
            s_EngineLogger->flush_on(spdlog::level::trace);
            spdlog::register_logger(s_DumpLogger);
        }

        CORVUS_CORE_INFO("Log successfully initialized!");
    }

} // namespace Corvus