#include "EventQueue.hpp"

#include <Peanut/Core/Assert.hpp>

namespace pn {

EventQueue::~EventQueue()
{
    if (!m_events.empty()) {
        PN_CORE_WARN("Event queue contains {} unhandled events upon destruction", m_events.size());
    }
}

void EventQueue::Push(const Event& event)
{
    m_events.emplace(std::unique_ptr<Event>(event.Copy()));
}

std::unique_ptr<Event> EventQueue::Pop()
{
    PN_CORE_ASSERT(!m_events.empty(), "Trying to get event from empty event queue");

    std::unique_ptr<Event> event = std::move(m_events.front());
    m_events.pop();
    return event;
}

}
