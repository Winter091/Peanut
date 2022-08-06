#include "OpenGLVertexArray.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/TimeProfiler.hpp>

#include <glad/glad.h>

#include <unordered_set>

namespace pn
{

OpenGLVertexArray::OpenGLVertexArray(const VertexArrayDescription& description)
{
    glCreateVertexArrays(1, &m_vaoHandle);

    for (const auto& vertexBuffer : description.VertexBuffers) {
        AddVertexBuffer(vertexBuffer);
    }

    if (description.IndexBuffer) {
        SetIndexBuffer(description.IndexBuffer);
    }

    m_shader = description.Shader;
}
    
OpenGLVertexArray::~OpenGLVertexArray()
{
    glDeleteVertexArrays(1, &m_vaoHandle);
}

void OpenGLVertexArray::Bind()
{
    glBindVertexArray(m_vaoHandle);
    m_shader->Bind();
}

void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
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

void OpenGLVertexArray::UpdateInstanceCount(const VertexBuffer& vertexBuffer)
{
    PN_PROFILE_FUNCTION();

    if (m_instanceCount == 0) {
        m_instanceCount = vertexBuffer.GetVertexCount();
        return;
    }

    if (vertexBuffer.GetVertexCount() != m_instanceCount) {
        PN_CORE_ASSERT(false, "Instance counts are not equal for vertex buffers in signle vertex array");
    }
}

void OpenGLVertexArray::ProcessVertexBufferLayout(VertexBuffer* vertexBuffer, uint32_t bindingIndex)
{
    PN_PROFILE_FUNCTION();

    const auto& layout = vertexBuffer->GetLayout();
    const auto& layoutElements = layout->GetElements();

    for (const auto& elem : layoutElements) {
        glVertexAttribFormat(
            elem.index, elem.count, MapToGLType(elem.type), 
            false, static_cast<GLuint>(elem.offset));

        glVertexAttribBinding(elem.index, bindingIndex);

        glEnableVertexAttribArray(elem.index);
    }
}

uint32_t OpenGLVertexArray::MapToGLType(BufferLayoutElementType type) const
{
    switch (type) {
        case BufferLayoutElementType::Int8:     return GL_BYTE;
        case BufferLayoutElementType::Int16:    return GL_SHORT;
        case BufferLayoutElementType::Int32:    return GL_INT;
        case BufferLayoutElementType::Uint8:    return GL_UNSIGNED_BYTE;
        case BufferLayoutElementType::Uint16:   return GL_UNSIGNED_SHORT;
        case BufferLayoutElementType::Uint32:   return GL_UNSIGNED_INT;
        case BufferLayoutElementType::Float:    return GL_FLOAT;
        default:                                break;
    }

    PN_CORE_ASSERT(false, "Unknown element type: {}", static_cast<uint32_t>(type));
    return 0u;
}

void OpenGLVertexArray::AssertAllAttributeIndicesAreUnique() const
{
    PN_PROFILE_FUNCTION();

    std::unordered_set<uint32_t> seenIndices;

    for (const auto& buffer : m_vertexBuffers) {
        const auto& bufferAttributes = buffer->GetLayout()->GetElements();

        for (const BufferLayoutElement& elem : bufferAttributes) {
            if (!seenIndices.insert(elem.index).second) {
                PN_CORE_ASSERT(false, "At least 2 attributes in vertex array have the same index = {}", elem.index);
            }
        }
    }
}

void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
    m_indexBuffer = indexBuffer;

    uint32_t handle = indexBuffer ? *(uint32_t*)indexBuffer->GetNativeHandle() : 0;

    glBindVertexArray(m_vaoHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
}

uint32_t OpenGLVertexArray::GetVertexCount() const 
{
    PN_CORE_ASSERT(!m_vertexBuffers.empty(), "No vertex buffers are bound to vertex array");
    return m_vertexBuffers.front()->GetVertexCount();
}

uint32_t OpenGLVertexArray::GetIndexCount() const 
{
    PN_CORE_ASSERT(m_indexBuffer, "Index buffer is not set");
    return m_indexBuffer->GetIndexCount();
}

IndexBufferDataFormat OpenGLVertexArray::GetIndexDataFormat() const 
{
    PN_CORE_ASSERT(m_indexBuffer, "Index buffer is not set");
    return m_indexBuffer->GetDataFormat();
}

uint32_t OpenGLVertexArray::GetInstanceCount() const 
{
    return m_instanceCount == 0 ? 1 : m_instanceCount;
}

}