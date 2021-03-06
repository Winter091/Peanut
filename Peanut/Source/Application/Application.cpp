#include <Peanut/Application/Application.hpp>

#include <Core/MethodBinding.hpp>
#include <Peanut/Events/WindowEvents.hpp>
#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/TimeProfiler.hpp>

namespace pn
{

Application::Application(const WindowSettings& settings)
{
    PN_PROFILE_FUNCTION();

    m_window = Window::Create(settings);
    m_window->SetEventCallbackFunc(PN_BIND_METHOD_CALL(MainOnEvent));
    RenderCommand::SetViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());
}

void Application::Run()
{
    PN_PROFILE_FUNCTION();

    while (!m_window->ShouldClose()) {
        MainOnUpdate();
    }
}

void Application::MainOnEvent(Event& event)
{
    PN_PROFILE_FUNCTION();

    m_eventQueue.Push(event);
    event.Dispatch<EventType::WindowSizeChanged>(PN_BIND_METHOD_CALL(OnWindowResize));
}

bool Application::OnWindowResize(Event& event)
{
    PN_PROFILE_FUNCTION();

    auto& resizeEvent = dynamic_cast<WindowSizeChangedEvent&>(event);
    m_window->OnResize(resizeEvent.GetWidth(), resizeEvent.GetHeight());
    RenderCommand::SetViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());
    return true;
}

void Application::MainOnUpdate()
{
    PN_PROFILE_FUNCTION();

    SendEvents();
    OnUpdate();
    m_window->Update();
}

void Application::SendEvents()
{
    PN_PROFILE_FUNCTION();

    while (m_eventQueue.HasEvents()) {
        std::unique_ptr<Event> event = m_eventQueue.Pop();
        OnEvent(*event);
    }
}

}