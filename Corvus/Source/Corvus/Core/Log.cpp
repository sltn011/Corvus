#include "Corvus/Core/Log.h"

#pragma warning(push, 0)
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#pragma warning(pop)

#include <vector>

namespace Corvus 
{

    std::shared_ptr<spdlog::logger> Log::s_EngineLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    Log::Log()
    {

    }

    Log::~Log()
    {

    }

    void Log::Init()
    {
        std::vector<spdlog::sink_ptr> LogSinks;
        LogSinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        LogSinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Corvus.log", true));

        for (spdlog::sink_ptr &Sink : LogSinks) {
            Sink->set_pattern("%^[%D %T][%l] %n: %v%$"); // COLOR([MM/DD/YY HH:MM:SS][loglevel] LogName: Message)
        }

        s_EngineLogger = std::make_shared<spdlog::logger>("CORVUS", LogSinks.begin(), LogSinks.end());
        s_EngineLogger->set_level(spdlog::level::trace);
        s_EngineLogger->flush_on(spdlog::level::trace);
        spdlog::register_logger(s_EngineLogger);

        s_ClientLogger = std::make_shared<spdlog::logger>("APP", LogSinks.begin(), LogSinks.end());
        s_ClientLogger->set_level(spdlog::level::trace);
        s_ClientLogger->flush_on(spdlog::level::trace);
        spdlog::register_logger(s_ClientLogger);
    }

}