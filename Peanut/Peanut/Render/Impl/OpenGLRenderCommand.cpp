#include "OpenGLRenderCommand.hpp"

#include <Peanut/Core/Assert.hpp>
#include <glad/glad.h>

namespace pn {

OpenGLRenderCommand::OpenGLRenderCommand()
{
    PN_CORE_INFO("Using OpenGL Renderer");
}

void OpenGLRenderCommand::ClearColor()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderCommand::ClearDepth()
{
    glClear(GL_DEPTH_BUFFER_BIT);
}

}