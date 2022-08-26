#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

namespace pn {

    class Log
    {
    public:
        static void Init();
        static void Shutdown();

        static spdlog::logger* GetCoreLogger();
        static spdlog::logger* GetClientLogger();

        static void SetCoreLoggerLever(spdlog::level::level_enum level) { m_coreLogger->set_level(level); }
        static void SetClientLoggerLever(spdlog::level::level_enum level) { m_clientLogger->set_level(level); }

    private:
        static bool s_isInitialized;

        static std::unique_ptr<spdlog::logger> m_coreLogger;
        static std::unique_ptr<spdlog::logger> m_clientLogger;
    };

}

#define PN_CORE_TRACE(...)      ::pn::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PN_CORE_DEBUG(...)      ::pn::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define PN_CORE_INFO(...)       ::pn::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PN_CORE_WARN(...)       ::pn::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PN_CORE_ERROR(...)      ::pn::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PN_CORE_CRITICAL(...)   ::pn::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define PN_CLIENT_TRACE(...)    ::pn::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PN_CLIENT_DEBUG(...)    ::pn::Log::GetClientLogger()->debug(__VA_ARGS__)
#define PN_CLIENT_INFO(...)     ::pn::Log::GetClientLogger()->info(__VA_ARGS__)
#define PN_CLIENT_WARN(...)     ::pn::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PN_CLIENT_ERROR(...)    ::pn::Log::GetClientLogger()->error(__VA_ARGS__)
#define PN_CLIENT_CRITICAL(...) ::pn::Log::GetClientLogger()->critical(__VA_ARGS__)
