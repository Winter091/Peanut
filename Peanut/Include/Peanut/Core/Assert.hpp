#pragma once

#include <Peanut/Core/Log.hpp>
#include <filesystem>

#if defined(PN_DEBUG)
    #define PN_ASSERT_IMPL(logName, cond, fmt, ...) \
        do { \
            if (!(cond)) { \
                PN_ ##logName ##_CRITICAL("Assertion error: " fmt, ##__VA_ARGS__); \
                PN_ ##logName ##_CRITICAL("\tFile: {}", ::std::filesystem::path(__FILE__).filename().string()); \
                PN_ ##logName ##_CRITICAL("\tLine: {}", __LINE__); \
                std::terminate(); \
            } \
        } while (false)

    #define PN_CORE_ASSERT(cond, fmt, ...) PN_ASSERT_IMPL(CORE, cond, fmt, ##__VA_ARGS__)
    #define PN_CLIENT_ASSERT(cond, fmt, ...) PN_ASSERT_IMPL(CLIENT, cond, fmt, ##__VA_ARGS__)
#else
    #define PN_CORE_ASSERT(cond, fmt, ...)
    #define PN_CLIENT_ASSERT(cond, fmt, ...)
#endif
