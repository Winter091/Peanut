#include <Peanut/Application/Application.hpp>

#include <Core/MethodBinding.hpp>
#include <Peanut/Events/WindowEvents.hpp>
#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/2D/Render/Renderer2D.hpp>

namespace pn
{

Application::Application(const WindowSettings& settings)
{
    m_window = Window::Create(settings);
    m_window->SetEventCallbackFunc(PN_BIND_METHOD_CALL(MainOnEvent));
    RenderCommand::SetViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());
    pn::Renderer2D::Init();
}

Application::~Application()
{
    pn::Renderer2D::Shutdown();   
}

void Application::Run()
{
    while (!m_window->ShouldClose()) {
        MainOnUpdate();
    }
}

void Application::MainOnEvent(Event& event)
{
    m_eventQueue.Push(event);
    event.Dispatch<EventType::WindowSizeChanged>(PN_BIND_METHOD_CALL(OnWindowResize));
}

bool Application::OnWindowResize(Event& event)
{
    auto& resizeEvent = dynamic_cast<WindowSizeChangedEvent&>(event);
    m_window->OnResize(resizeEvent.GetWidth(), resizeEvent.GetHeight());
    RenderCommand::SetViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());
    return true;
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