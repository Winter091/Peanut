#include "Application.hpp"


namespace pn
{

Application::Application(int width, int height, std::string title)
    : m_window(Window::Create(width, height, std::move(title)))
{
    m_window->SetEventCallbackFunc(std::bind(&Application::MainOnEvent, this, std::placeholders::_1));
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