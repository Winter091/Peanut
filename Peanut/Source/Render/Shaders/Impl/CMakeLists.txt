
if (PN_RENDERING_DX11)
    target_sources(
        Peanut PRIVATE
        Dx11Shader.cpp
        Dx11ShaderInputLayout.cpp
    )
elseif (PN_RENDERING_OPENGL)
    target_sources(
        Peanut PRIVATE
        OpenGLShader.cpp
    )
endif()