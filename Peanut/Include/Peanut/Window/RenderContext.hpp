#pragma once

#include <memory>

namespace pn {

    class Window;
    struct WindowSizeSettings;

    class RenderContext
    {
    public:
        RenderContext(const RenderContext&) = delete;
        RenderContext& operator=(const RenderContext&) = delete;
        virtual ~RenderContext() = default;

        static std::unique_ptr<RenderContext> Create();

        virtual void PreWindowSetup() = 0;
        virtual void PostWindowSetup(Window& window) = 0;
        virtual void SetCurrentContext(Window& window) = 0;
        virtual void SetSwapInterval(int interval) = 0;
        virtual void SwapBuffers(Window& window) = 0;
        virtual void OnWindowResize(const WindowSizeSettings& settings) = 0;

    protected:
        RenderContext() = default;
    };

}
