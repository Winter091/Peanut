#ifndef __OPENGLGLFWRENDERCONTEXT_H__
#define __OPENGLGLFWRENDERCONTEXT_H__

#include <Peanut/Window/RenderContext/RenderContext.hpp>

namespace pn {

class OpenGLGLFWRenderContext : public RenderContext
{
public:
    OpenGLGLFWRenderContext();
    OpenGLGLFWRenderContext(const OpenGLGLFWRenderContext&) = delete;
    OpenGLGLFWRenderContext& operator=(const OpenGLGLFWRenderContext&) = delete;
    ~OpenGLGLFWRenderContext() override = default;

    void Setup() override;
    void SetCurrentContext(Window& window) override;
    void SwapBuffers(Window& window) override;
};

}

#endif