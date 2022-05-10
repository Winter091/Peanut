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
    virtual ~RenderContext() {}

    static std::unique_ptr<RenderContext> Create();

    virtual void SetWindow(Window& window) = 0;
    virtual void SwapBuffers(Window& window) = 0;

protected:
    RenderContext() = default;
};

}

#endif