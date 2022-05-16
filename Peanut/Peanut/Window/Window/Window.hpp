#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <Peanut/Window/RenderContext/RenderContext.hpp>
#include <Peanut/Window/Window/WindowProvider.hpp>
#include <Peanut/Events/Event.hpp>
#include <Peanut/Window/Window/WindowSettings.hpp>
#include <memory>
#include <string>
#include <functional>

namespace pn {

class Window 
{
public:
    using EventCallbackFunc = std::function<void(const Event&)>;

    Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    virtual ~Window() = default;

    static void Init();
    static std::unique_ptr<Window> Create(const WindowSettings& settings);
    static WindowProvider GetWindowProvider();

    virtual void MakeContextCurrent() = 0;
    virtual void SetEventCallbackFunc(const EventCallbackFunc& func) = 0;
    virtual void Update() = 0;
    virtual bool ShouldClose() const = 0;

    virtual void* GetNativeHandle() const = 0;
    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;
    
    virtual const std::string& GetTitle() const = 0;
    virtual void SetTitle(const std::string& title) = 0;

    virtual void SetSwapInterval(int interval) = 0;

    RenderContext& GetRenderContext() { return *m_renderContext; }

private:
    std::unique_ptr<RenderContext> m_renderContext;
    static WindowProvider s_windowProvider;
    static bool s_isInitialized;
};

}

#endif