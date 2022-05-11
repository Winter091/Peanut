#include "Application.hpp"

#include "Core.hpp"

namespace pn
{

Application::Application(int width, int height, std::string title)
    : m_window(Window::Create(width, height, std::move(title)))
{
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
    OnEvent(event);
}

void Application::MainOnUpdate()
{
    OnUpdate();
    m_window->Update();
}

}