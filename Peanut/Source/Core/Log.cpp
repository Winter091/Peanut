#include <Peanut/Core/Log.hpp>

// Stick to C asserts here, because Peanut asserts 
// don't work without this logger being initialized

namespace pn {

bool Log::s_isInitialized = false;
std::unique_ptr<spdlog::logger> Log::m_coreLogger;
std::unique_ptr<spdlog::logger> Log::m_clientLogger;

void Log::Init()
{
    assert(!s_isInitialized && "Logger should be initialized only once");
    s_isInitialized = true;

    std::vector<spdlog::sink_ptr> sinks = {
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
        std::make_shared<spdlog::sinks::basic_file_sink_mt>("Logs/Log.txt", true)
    };

    sinks[0]->set_pattern("[%H:%M:%S.%e] [%n] [%^%l%$] %v");
    sinks[1]->set_pattern("[%H:%M:%S.%e] [%n] [%l] %v");

    m_coreLogger = std::make_unique<spdlog::logger>("CORE", sinks.begin(), sinks.end());
    m_coreLogger->set_level(spdlog::level::trace);
    m_coreLogger->flush_on(spdlog::level::trace);

    m_clientLogger = std::make_unique<spdlog::logger>("CLIENT", sinks.begin(), sinks.end());
    m_clientLogger->set_level(spdlog::level::trace);
    m_clientLogger->flush_on(spdlog::level::trace);
}

spdlog::logger& Log::GetCoreLogger()
{
    assert(s_isInitialized && "Logger should be initialized before usage");
    return *m_coreLogger;
}

spdlog::logger& Log::GetClientLogger()
{
    assert(s_isInitialized && "Logger should be initialized before usage");
    return *m_clientLogger;
}

}
