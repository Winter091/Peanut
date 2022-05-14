#include "OpenGLRenderCommand.hpp"

#include <Peanut/Core/Assert.hpp>
#include <glad/glad.h>

namespace pn {

OpenGLRenderCommand::OpenGLRenderCommand()
{
    PN_CORE_INFO("Using OpenGL Renderer");
}

void OpenGLRenderCommand::SetClearColor(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRenderCommand::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderCommand::DrawArrays(uint32_t first, uint32_t count)
{
    glDrawArrays(GL_TRIANGLES, static_cast<int32_t>(first), static_cast<int32_t>(count));
}

}