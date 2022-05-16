#include "Application.hpp"

#include <Peanut/Core/Core.hpp>
#include <Peanut/Render/Commands/RenderCommand.hpp>

namespace pn
{

Application::Application(const WindowSettings& settings)
{
    m_window = Window::Create(settings);
    m_window->SetEventCallbackFunc(PN_BIND_METHOD_CALL(MainOnEvent));

    RenderCommand::SetViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());
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