if (PN_RENDERING_DX11)
    target_sources(
        Peanut PRIVATE
        Dx11RenderCommand.cpp
    )
elseif (PN_RENDERING_OPENGL)
    target_sources(
        Peanut PRIVATE
        OpenGLRenderCommand.cpp
    )
endif()
