#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <Window/RenderContext/RenderContext.hpp>
#include <Window/Window/EWindowProvider.hpp>
#include <memory>
#include <string>

namespace pn {

class Window 
{
public:
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    virtual ~Window() = default;

    static void Init();
    static std::unique_ptr<Window> Create(int width, int height, const char* title);
    static EWindowProvider GetWindowProvider();

    virtual void MakeContextCurrent() = 0;
    virtual void SetEventCallbackFunc() = 0;
    virtual void Update() = 0;
    virtual bool ShouldClose() const = 0;
    virtual void* GetNativeHandle() const = 0;

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

protected:
    Window(int width, int height, std::string title);

protected:
    int m_width;
    int m_height;
    std::string m_title;

    std::unique_ptr<RenderContext> m_renderContext;

private:
    static EWindowProvider s_windowProvider;
    static bool s_isInitialized;
};

}

#endif