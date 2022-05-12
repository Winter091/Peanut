#ifndef __EVENT_H__
#define __EVENT_H__

#include <Peanut/Events/EventType.hpp>
#include <string>
#include <Peanut/Core/Assert.hpp>

namespace pn {

class Event
{
public:
    virtual ~Event() = default;

    virtual EventType GetType() const = 0;
    virtual const char* GetStringType() const = 0;
    virtual std::string ToString() const = 0;
    virtual Event* Copy() const = 0;

    void SetIsProcessed(bool value) { m_isProcessed = value; }
    bool GetIsProcessed() const { return m_isProcessed; }

private:
    bool m_isProcessed = false;
};

#define PN_DEFINE_EVENT_TYPE(type) \
    EventType GetType() const override { return EventType::type; } \
    const char* GetStringType() const override { return #type; }

}

#endif