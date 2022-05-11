#ifndef __CORE_H__
#define __CORE_H__

#define PN_BIND_METHOD_CALL(method) \
    [this](auto&&... args) -> decltype(auto) { \
        return this->method(std::forward<decltype(args)...>(args...)); \
    }

#endif