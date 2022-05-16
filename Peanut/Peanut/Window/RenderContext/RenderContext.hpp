#ifndef __RENDERCONTEXT_H__
#define __RENDERCONTEXT_H__

#include <memory>

namespace pn {

class Window;

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
    virtual void SwapBuffers(Window& window) = 0;
    virtual void OnWindowResize(Window& window) = 0;

protected:
    RenderContext() = default;
};

}

#endif