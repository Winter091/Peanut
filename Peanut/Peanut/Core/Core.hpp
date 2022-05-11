#ifndef __CORE_H__
#define __CORE_H__

#include <Peanut/Core/PlatformDetection.hpp>
#include <Peanut/Core/Log.hpp>
#include <Peanut/Core/Assert.hpp>

#define PN_BIND_METHOD_CALL(method) \
    [this](auto&&... args) -> decltype(auto) { \
        return this->method(std::forward<decltype(args)...>(args...)); \
    }

#endif