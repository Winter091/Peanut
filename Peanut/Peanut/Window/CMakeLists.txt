target_sources(
    Peanut PRIVATE

    RenderContext/RenderContext.cpp
    RenderContext/Impl/DX11GLFWRenderContext.cpp
    RenderContext/Impl/OpenGLGLFWRenderContext.cpp

    Window/Window.cpp
    Window/Impl/GLFWWindow.cpp
)