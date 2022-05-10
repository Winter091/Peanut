#ifndef __OPENGLGLFWRENDERCONTEXT_H__
#define __OPENGLGLFWRENDERCONTEXT_H__

#include <Window/RenderContext/RenderContext.hpp>

namespace pn {

class OpenGLGLFWRenderContext : public RenderContext
{
public:
    OpenGLGLFWRenderContext();
    OpenGLGLFWRenderContext(const OpenGLGLFWRenderContext&) = delete;
    OpenGLGLFWRenderContext& operator=(const OpenGLGLFWRenderContext&) = delete;
    virtual ~OpenGLGLFWRenderContext() override;

    virtual void Setup() override;
    virtual void SetCurrentContext(Window& window) override;
    virtual void SwapBuffers(Window& window) override;
};

}

#endif