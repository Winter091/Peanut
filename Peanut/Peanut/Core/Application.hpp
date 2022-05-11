#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <Peanut/Window/Window/Window.hpp>
#include <Peanut/Events/Event.hpp>
#include <memory>

namespace pn {

class Application
{
public:
    Application(int width, int height, std::string title);
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    virtual ~Application() = default;

    static void Init();

    Window& GetWindow() { return *m_window; }

    virtual void OnEvent(const Event& event) = 0;
    virtual void OnUpdate() = 0;

    void Run();

    static Application* CreateApplication();

private:
    std::unique_ptr<Window> m_window;

private:
    void MainOnEvent(const Event& event);
    void MainOnUpdate();
};

}

#endif