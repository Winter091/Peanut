#include "OpenGLRenderCommand.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/TimeProfiler.hpp>
#include <Render/Buffers/Impl/OpenGLIndexBuffer.hpp>

#include <glad/glad.h>

namespace pn {

void OpenGLRenderCommand::SetClearColor(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRenderCommand::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderCommand::SetViewport(int32_t leftX, int32_t bottomY, uint32_t width, uint32_t height)
{
    glViewport(leftX, bottomY, width, height);
}

void OpenGLRenderCommand::DrawArrays(std::shared_ptr<VertexArray>& pipelineState, uint32_t count)
{
    PN_PROFILE_FUNCTION();

    if (count == 0) {
        count = pipelineState->GetVertexCount();
    }

    pipelineState->Bind();
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(count));
}

void OpenGLRenderCommand::DrawIndexed(std::shared_ptr<VertexArray>& pipelineState, uint32_t count)
{
    PN_PROFILE_FUNCTION();

    if (count == 0) {
        count = pipelineState->GetIndexCount();
    }

    uint32_t dataType = GetGLDataType(pipelineState->GetIndexDataFormat());

    pipelineState->Bind();
    glDrawElements(GL_TRIANGLES, count, dataType, nullptr);
}

void OpenGLRenderCommand::DrawArraysInstanced(std::shared_ptr<VertexArray>& pipelineState, uint32_t count, uint32_t instanceCount)
{
    PN_PROFILE_FUNCTION();

    if (count == 0) {
        count = pipelineState->GetVertexCount();
    }

    if (instanceCount == 0) {
        instanceCount = pipelineState->GetInstanceCount();
    }

    pipelineState->Bind();
    glDrawArraysInstanced(GL_TRIANGLES, 0, static_cast<GLsizei>(count), instanceCount);
}

void OpenGLRenderCommand::DrawIndexedInstanced(std::shared_ptr<VertexArray>& pipelineState, uint32_t count, uint32_t instanceCount)
{
    PN_PROFILE_FUNCTION();

    if (count == 0) {
        count = pipelineState->GetIndexCount();
    }

    if (instanceCount == 0) {
        instanceCount = pipelineState->GetInstanceCount();
    }

    uint32_t dataType = GetGLDataType(pipelineState->GetIndexDataFormat());

    pipelineState->Bind();
    glDrawElementsInstanced(GL_TRIANGLES, count, dataType, nullptr, instanceCount);
}

uint32_t OpenGLRenderCommand::GetGLDataType(IndexBufferDataFormat format)
{
    switch (format) {
        case IndexBufferDataFormat::Uint16: return GL_UNSIGNED_SHORT;
        case IndexBufferDataFormat::Uint32: return GL_UNSIGNED_INT;
        default: break;
    }

    PN_CORE_ASSERT(false, "Unknown index buffer data format: {}", static_cast<uint32_t>(format));
    return 0u;
}

}