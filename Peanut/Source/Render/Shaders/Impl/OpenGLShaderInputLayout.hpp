#pragma once

#include <Peanut/Render/Shaders/ShaderInputLayout.hpp>

namespace pn {

// Yeah, we don't really need it for OpenGL Rendering because in OpenGL you
// have to generate layout every time you create new VAO
class OpenGLShaderInputLayout final : public ShaderInputLayout
{
public:
    OpenGLShaderInputLayout() {}
    ~OpenGLShaderInputLayout() override {}
};

}