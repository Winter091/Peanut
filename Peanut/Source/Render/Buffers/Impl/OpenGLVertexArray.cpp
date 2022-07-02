#include "OpenGLVertexArray.hpp"

#include <Peanut/Core/Assert.hpp>
#include <glad/glad.h>

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

    int bindingIndex = m_vertexBuffers.size();
    int divisor = (usage == BufferDataUsage::PerVertex ? 0 : 1);
    vertexBuffer->BindToBindingIndex(bindingIndex);
    glVertexBindingDivisor(bindingIndex, divisor);

    ProcessVertexBufferLayout(vertexBuffer.get(), bindingIndex);
    m_vertexBuffers.push_back(vertexBuffer);

    Unbind();
}

void OpenGLVertexArray::ProcessVertexBufferLayout(VertexBuffer* vertexBuffer, int bindingIndex)
{
    const auto& layout = vertexBuffer->GetLayout();
    const auto& layoutElements = layout->GetElements();

    for (uint32_t i = 0; i < layoutElements.size(); i++) {
        const auto& elem = layoutElements[i];

        glVertexAttribFormat(elem.index, elem.count, MapToGLType(elem.type), elem.isNormalized, elem.offset);
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