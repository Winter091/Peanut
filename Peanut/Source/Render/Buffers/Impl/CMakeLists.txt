if (PN_RENDERING_DX11)
    target_sources(
        Peanut PRIVATE
        Dx11ConstantBuffer.cpp
        Dx11IndexBuffer.cpp
        Dx11VertexArray.cpp
        Dx11VertexBuffer.cpp
    )
elseif (PN_RENDERING_OPENGL)
    target_sources(
        Peanut PRIVATE
        OpenGLConstantBuffer.cpp
        OpenGLIndexBuffer.cpp
        OpenGLVertexArray.cpp
        OpenGLVertexBuffer.cpp
    )
endif()
