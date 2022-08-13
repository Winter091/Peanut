if (PN_RENDERING_DX11)
    target_sources(
        Peanut PRIVATE
        Dx11ConstantBuffer.cpp
        Dx11IndexBuffer.cpp
        Dx11PipelineState.cpp
        Dx11VertexBuffer.cpp
    )
elseif (PN_RENDERING_OPENGL)
    target_sources(
        Peanut PRIVATE
        OpenGLConstantBuffer.cpp
        OpenGLIndexBuffer.cpp
        OpenGLPipelineState.cpp
        OpenGLVertexBuffer.cpp
    )
endif()
