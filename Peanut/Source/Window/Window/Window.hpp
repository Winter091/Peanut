#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <Window/RenderContext/RenderContext.hpp>
#include <Window/Window/EWindowProvider.hpp>
#include <memory>

namespace pn {

class Window 
{
public:
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    virtual ~Window() {}

    static void Init();
    static std::unique_ptr<Window> Create();
    static EWindowProvider GetWindowProvider();

    virtual void SetEventCallbackFunc() = 0;
    virtual void Update() = 0;

protected:
    Window();

protected:
    std::unique_ptr<RenderContext> m_renderContext;

private:
    static EWindowProvider s_windowProvider;
    const static bool s_initted;
};

}

#endif