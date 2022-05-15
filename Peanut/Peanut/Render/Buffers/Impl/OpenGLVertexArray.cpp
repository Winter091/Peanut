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

void OpenGLVertexArray::SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
    Bind();
    m_vertexBuffer = vertexBuffer;
    ProcessVertexBufferLayout();
    Unbind();
}

void OpenGLVertexArray::ProcessVertexBufferLayout()
{
    m_vertexBuffer->Bind();
    
    const auto& layout = m_vertexBuffer->GetLayout();
    const auto& layoutElements = layout->GetElements();

    for (uint32_t i = 0; i < layoutElements.size(); i++) {
        ProcessVertexBufferLayoutElement(layoutElements[i], i, layout->GetStride());
    }
}

void OpenGLVertexArray::ProcessVertexBufferLayoutElement(
    const BufferLayoutElement& elem, uint32_t index, uint32_t stride
)
{
    switch (elem.type) {
        case BufferLayoutElementType::Int8:
        case BufferLayoutElementType::Int16:
        case BufferLayoutElementType::Int32:
        case BufferLayoutElementType::Uint8:
        case BufferLayoutElementType::Uint16:
        case BufferLayoutElementType::Uint32:
        {
            glVertexAttribIPointer(
                index, static_cast<int32_t>(elem.count), MapToGLType(elem.type), 
                static_cast<int32_t>(stride), (const void*)elem.offset
            );
            break;
        }
        case BufferLayoutElementType::Float:
        case BufferLayoutElementType::Double:
        {
            glVertexAttribPointer(
                index, static_cast<int32_t>(elem.count), MapToGLType(elem.type), elem.isNormalized, 
                static_cast<int32_t>(stride), (const void*)elem.offset
            );
            break;
        }
        default:
        {
            PN_CORE_ASSERT(false, "Unknown element type: {}", static_cast<uint32_t>(elem.type));
            break;
        }
    }

    glEnableVertexAttribArray(index);
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

uint32_t OpenGLVertexArray::GetSize() const 
{
    PN_CORE_ASSERT(m_vertexBuffer, "Vertex buffer is not set");
    return m_vertexBuffer->GetSize();
}

uint32_t OpenGLVertexArray::GetVertexCount() const 
{
    PN_CORE_ASSERT(m_vertexBuffer, "Vertex buffer is not set");
    return m_vertexBuffer->GetVertexCount();
}

}