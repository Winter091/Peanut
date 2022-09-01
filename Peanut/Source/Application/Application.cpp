#include <Peanut/Application/Application.hpp>

#include <Core/MethodBinding.hpp>
#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/TimeProfiler.hpp>

namespace pn
{

    static Application* s_instance = nullptr;

    Application& Application::GetInstance()
    {
        PN_CORE_ASSERT(s_instance, "Trying to get application, but it's not initialized");
        return *s_instance;
    }

    Application::Application(const WindowSettings& settings)
    {
        PN_PROFILE_FUNCTION();

        PN_CORE_ASSERT(!s_instance, "Application alrealy has instance");
        s_instance = this;

        m_window = Window::Create(settings);
        m_window->SetEventCallbackFunc(PN_BIND_METHOD_CALL(MainOnEvent));
        RenderCommand::SetViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());
    }

    Application::~Application()
    {
        PN_CORE_ASSERT(s_instance, "Application doesn't have instance");
        s_instance = nullptr;
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
        event.Dispatch<WindowSizeChangedEvent>(PN_BIND_METHOD_CALL(OnWindowResize));
    }

    bool Application::OnWindowResize(WindowSizeChangedEvent& resizeEvent)
    {
        PN_PROFILE_FUNCTION();

        RenderCommand::SetViewport(0, 0, resizeEvent.GetWidth(), resizeEvent.GetHeight());
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