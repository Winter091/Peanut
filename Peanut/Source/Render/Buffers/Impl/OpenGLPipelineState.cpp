#include "OpenGLPipelineState.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/TimeProfiler.hpp>
#include <Render/Buffers/Impl/OpenGLEnumConversions.hpp>

#include <glad/glad.h>

#include <unordered_set>

namespace pn
{

OpenGLPipelineState::OpenGLPipelineState(const PipelineStateDescription& description)
    : m_constantBuffers(description.ConstantBuffers)
    , m_shader(description.Shader)
{
    glCreateVertexArrays(1, &m_vaoHandle);

    for (const auto& vertexBuffer : description.VertexBuffers) {
        AddVertexBuffer(vertexBuffer);
    }

    if (description.IndexBuffer) {
        SetIndexBuffer(description.IndexBuffer);
    }
}
    
OpenGLPipelineState::~OpenGLPipelineState()
{
    glDeleteVertexArrays(1, &m_vaoHandle);
}

void OpenGLPipelineState::Bind()
{
    glBindVertexArray(m_vaoHandle);

    for (uint32_t i = 0; i < m_constantBuffers.size(); i++) {
        uint32_t handle = *(uint32_t*)m_constantBuffers[i]->GetNativeHandle();
        glBindBufferBase(GL_UNIFORM_BUFFER, i, handle);
    }

    m_shader->Bind();
}

void OpenGLPipelineState::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
    PN_PROFILE_FUNCTION();

    glBindVertexArray(m_vaoHandle);

    uint32_t bindingIndex = static_cast<uint32_t>(m_vertexBuffers.size());
    uint32_t vertexBufferHandle = *(uint32_t*)vertexBuffer->GetNativeHandle();
    glBindVertexBuffer(bindingIndex, vertexBufferHandle, 0, vertexBuffer->GetVertexSize());

    if (vertexBuffer->GetLayout()->GetUsage() == pn::BufferLayoutAttributeUsage::PerVertex) {
        glVertexBindingDivisor(bindingIndex, 0);
    } else {
        glVertexBindingDivisor(bindingIndex, 1);
        UpdateInstanceCount(*vertexBuffer);
    }

    ProcessVertexBufferLayout(vertexBuffer.get(), bindingIndex);
    m_vertexBuffers.push_back(vertexBuffer);

#if defined(PN_DEBUG)
    AssertAllAttributeIndicesAreUnique();
#endif
}

void OpenGLPipelineState::UpdateInstanceCount(const VertexBuffer& vertexBuffer)
{
    PN_PROFILE_FUNCTION();

    if (m_instanceCount == 0) {
        m_instanceCount = vertexBuffer.GetVertexCount();
        return;
    }

    if (vertexBuffer.GetVertexCount() != m_instanceCount) {
        PN_CORE_ASSERT(false, "Instance counts are not equal for vertex buffers in one pipeline state");
    }
}

void OpenGLPipelineState::ProcessVertexBufferLayout(VertexBuffer* vertexBuffer, uint32_t bindingIndex)
{
    PN_PROFILE_FUNCTION();

    const auto& layout = vertexBuffer->GetLayout();
    const auto& layoutElements = layout->GetElements();

    for (const auto& elem : layoutElements) {
        if (IsBufferLayoutElementTypeInt(elem.type)) {
            glVertexAttribIFormat(elem.index, elem.count, BufferLayoutElementTypeToGlType(elem.type), static_cast<GLuint>(elem.offset));
        }
        else {
            glVertexAttribFormat(elem.index, elem.count, BufferLayoutElementTypeToGlType(elem.type), false, static_cast<GLuint>(elem.offset));
        }

        glVertexAttribBinding(elem.index, bindingIndex);
        glEnableVertexAttribArray(elem.index);
    }
}

void OpenGLPipelineState::AssertAllAttributeIndicesAreUnique() const
{
    PN_PROFILE_FUNCTION();

    std::unordered_set<uint32_t> seenIndices;

    for (const auto& buffer : m_vertexBuffers) {
        const auto& bufferAttributes = buffer->GetLayout()->GetElements();

        for (const BufferLayoutElement& elem : bufferAttributes) {
            if (!seenIndices.insert(elem.index).second) {
                PN_CORE_ASSERT(false, "At least 2 attributes in pipeline state have the same index = {}", elem.index);
            }
        }
    }
}

void OpenGLPipelineState::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
    m_indexBuffer = indexBuffer;

    uint32_t handle = indexBuffer ? *(uint32_t*)indexBuffer->GetNativeHandle() : 0;

    glBindVertexArray(m_vaoHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
}

uint32_t OpenGLPipelineState::GetVertexCount() const 
{
    PN_CORE_ASSERT(!m_vertexBuffers.empty(), "No vertex buffers are bound to pipeline state");
    return m_vertexBuffers.front()->GetVertexCount();
}

uint32_t OpenGLPipelineState::GetIndexCount() const 
{
    PN_CORE_ASSERT(m_indexBuffer, "Index buffer is not set");
    return m_indexBuffer->GetIndexCount();
}

IndexBufferDataFormat OpenGLPipelineState::GetIndexDataFormat() const 
{
    PN_CORE_ASSERT(m_indexBuffer, "Index buffer is not set");
    return m_indexBuffer->GetDataFormat();
}

uint32_t OpenGLPipelineState::GetInstanceCount() const 
{
    return m_instanceCount == 0 ? 1 : m_instanceCount;
}

}