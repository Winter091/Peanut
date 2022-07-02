#include "OpenGLVertexArray.hpp"

#include <Peanut/Core/Assert.hpp>
#include <glad/glad.h>

#include <unordered_set>

namespace pn
{

OpenGLVertexArray::OpenGLVertexArray()
{
    glCreateVertexArrays(1, &m_handle);
}
    
OpenGLVertexArray::~OpenGLVertexArray()
{
    glDeleteVertexArrays(1, &m_handle);
}

void OpenGLVertexArray::Bind()
{
    glBindVertexArray(m_handle);
}

void OpenGLVertexArray::Unbind()
{
    glBindVertexArray(0u);
}

void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer, BufferDataUsage usage)
{
    Bind();
    vertexBuffer->Bind();

    uint32_t bindingIndex = static_cast<uint32_t>(m_vertexBuffers.size());
    uint32_t divisor = (usage == BufferDataUsage::PerVertex ? 0u : 1u);
    vertexBuffer->BindToBindingIndex(bindingIndex);
    glVertexBindingDivisor(bindingIndex, divisor);

    ProcessVertexBufferLayout(vertexBuffer.get(), bindingIndex);
    m_vertexBuffers.push_back(vertexBuffer);

    Unbind();

#if defined(PN_DEBUG)
    AssertAllAttributeIndicesAreUnique();
#endif
}

void OpenGLVertexArray::ProcessVertexBufferLayout(VertexBuffer* vertexBuffer, int bindingIndex)
{
    const auto& layout = vertexBuffer->GetLayout();
    const auto& layoutElements = layout->GetElements();

    for (const auto& elem : layoutElements) {
        glVertexAttribFormat(
            elem.index, elem.count, MapToGLType(elem.type), 
            elem.isNormalized, static_cast<GLuint>(elem.offset));

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
        case BufferLayoutElementType::Double:   return GL_DOUBLE;
        default:                                break;
    }

    PN_CORE_ASSERT(false, "Unknown element type: {}", static_cast<uint32_t>(type));
    return 0u;
}

void OpenGLVertexArray::AssertAllAttributeIndicesAreUnique() const
{
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

    Bind();
    indexBuffer->Bind();
    Unbind();
}

uint32_t OpenGLVertexArray::GetSize() const 
{
    PN_CORE_ASSERT(!m_vertexBuffers.empty(), "No vertex buffers are bound to vertex array");
    return m_vertexBuffers.front()->GetSize();
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

}