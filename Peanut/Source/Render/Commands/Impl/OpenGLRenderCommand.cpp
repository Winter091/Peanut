#include "OpenGLRenderCommand.hpp"

#include <Peanut/Core/Assert.hpp>
#include <glad/glad.h>

#include <Render/Buffers/Impl/OpenGLIndexBuffer.hpp>

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

void OpenGLRenderCommand::DrawArrays(std::shared_ptr<VertexArray>& vertexArray, uint32_t count)
{
    if (count == 0) {
        count = vertexArray->GetVertexCount();
    }

    vertexArray->Bind();
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(count));
}

void OpenGLRenderCommand::DrawIndexed(std::shared_ptr<VertexArray>& vertexArray, uint32_t count)
{
    if (count == 0) {
        count = vertexArray->GetIndexCount();
    }

    uint32_t dataType = GetGLDataType(vertexArray->GetIndexDataFormat());

    vertexArray->Bind();
    glDrawElements(GL_TRIANGLES, count, dataType, nullptr);
}

uint32_t OpenGLRenderCommand::GetGLDataType(IndexBufferDataFormat format)
{
    switch (format) {
        case IndexBufferDataFormat::Uint8:  return GL_UNSIGNED_BYTE;
        case IndexBufferDataFormat::Uint16: return GL_UNSIGNED_SHORT;
        case IndexBufferDataFormat::Uint32: return GL_UNSIGNED_INT;
        default: break;
    }

    PN_CORE_ASSERT(false, "Unknown index buffer data format: {}", static_cast<uint32_t>(format));
    return 0u;
}

}