if (PN_RENDERING_DX11)
    target_sources(
        Peanut PRIVATE
        Dx11Framebuffer.cpp
    )
elseif (PN_RENDERING_OPENGL)
    target_sources(
        Peanut PRIVATE
    )
endif()