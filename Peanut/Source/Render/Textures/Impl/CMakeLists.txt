if (PN_RENDERING_DX11)
    target_sources(
        Peanut PRIVATE
        Dx11TextureSampler.cpp
    )
elseif (PN_RENDERING_OPENGL)
    target_sources(
        Peanut PRIVATE
        OpenGLTexture2D.cpp
        OpenGLTextureSampler.cpp
    )
endif()
