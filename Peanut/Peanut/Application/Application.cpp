#include "Application.hpp"

#include <Peanut/Core/Core.hpp>

namespace pn
{

Application::Application(const ApplicationOptions& options)
{
    m_window = Window::Create(options.width, options.height, options.title);
    m_window->SetEventCallbackFunc(PN_BIND_METHOD_CALL(MainOnEvent));
}

void Application::Init()
{
    Window::Init();
}

void Application::Run()
{
    while (!m_window->ShouldClose()) {
        MainOnUpdate();
    }
}

void Application::MainOnEvent(const Event& event)
{
    m_eventQueue.Push(event);
}

void Application::MainOnUpdate()
{
    SendEvents();
    OnUpdate();
    m_window->Update();
}

void Application::SendEvents()
{
    while (m_eventQueue.HasEvents()) {
        std::unique_ptr<Event> event = m_eventQueue.Pop();
        OnEvent(*event);
    }
}

}