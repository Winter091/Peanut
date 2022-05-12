#ifndef __EVENTQUEUE_H__
#define __EVENTQUEUE_H__

#include <queue>
#include <memory>

#include <Peanut/Events/Event.hpp>

namespace pn {

class EventQueue
{
public:
    EventQueue() = default;
    EventQueue(const EventQueue&) = delete;
    EventQueue& operator=(const EventQueue&) = delete;
    ~EventQueue();

    void Push(const Event& event);
    bool HasEvents() const { return !m_events.empty(); }
    std::unique_ptr<Event> Pop();

private:
    std::queue<std::unique_ptr<Event>> m_events;
};

}

#endif