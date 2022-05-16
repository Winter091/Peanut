#ifndef __OPENGLGLFWRENDERCONTEXT_H__
#define __OPENGLGLFWRENDERCONTEXT_H__

#include <Peanut/Window/RenderContext/RenderContext.hpp>
#include <Peanut/Core/Log.hpp>

namespace pn {

class OpenGLGLFWRenderContext final : public RenderContext
{
public:
    OpenGLGLFWRenderContext();
    OpenGLGLFWRenderContext(const OpenGLGLFWRenderContext&) = delete;
    OpenGLGLFWRenderContext& operator=(const OpenGLGLFWRenderContext&) = delete;
    ~OpenGLGLFWRenderContext() override = default;

    void PreWindowSetup() override;
    void PostWindowSetup(Window& window) override;
    void SetCurrentContext(Window& window) override;
    void SwapBuffers(Window& window) override;
    void OnWindowResize(Window& window) override;

private:
    void InitializeGlad();
    void SetupDebugContext();
};

}

#endif