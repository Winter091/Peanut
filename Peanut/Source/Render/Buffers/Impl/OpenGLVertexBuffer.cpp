#include "OpenGLVertexBuffer.hpp"

#include <Peanut/Core/Assert.hpp>
#include <glad/glad.h>

namespace pn
{

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, const void* data)
    : m_handle(0)
    , m_size(size)
{
    PN_CORE_ASSERT(m_size > 0u, "Unable to create vertex buffer with size = 0");
    
    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, size, data, GL_MAP_WRITE_BIT);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    glDeleteBuffers(1, &m_handle);
}

void OpenGLVertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
}

void OpenGLVertexBuffer::BindToBindingIndex(uint32_t index)
{
    glBindVertexBuffer(index, m_handle, 0, GetVertexSize());
}

void OpenGLVertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0u);
}

void* OpenGLVertexBuffer::Map(BufferMapAccess access)
{
    void* buffer = glMapNamedBuffer(m_handle, ToGlMapAccess(access));
    PN_CORE_ASSERT(buffer, "Mapped buffer is nullptr");
    return buffer;
}

void OpenGLVertexBuffer::Unmap()
{
    glUnmapNamedBuffer(m_handle);
}

const std::shared_ptr<BufferLayout>& OpenGLVertexBuffer::GetLayout() const 
{
    PN_CORE_ASSERT(m_layout, "Layout is not specified");
    return m_layout;
}

void OpenGLVertexBuffer::SetLayout(const std::shared_ptr<BufferLayout>& layout)
{
    PN_CORE_ASSERT(layout, "Layout is nullptr");
    m_layout = layout;
}

uint32_t OpenGLVertexBuffer::GetVertexSize() const 
{
    PN_CORE_ASSERT(m_layout, "Layout is nullptr");
    return m_layout->GetStride();
}

uint32_t OpenGLVertexBuffer::GetVertexCount() const 
{
    PN_CORE_ASSERT(m_layout, "Layout is nullptr");
    return GetSize() / GetVertexSize();
}

uint32_t OpenGLVertexBuffer::ToGlMapAccess(BufferMapAccess access)
{
    switch (access) {
        case BufferMapAccess::ReadOnly:     return GL_READ_ONLY;
        case BufferMapAccess::WriteOnly:    return GL_WRITE_ONLY;
        case BufferMapAccess::ReadWrite:    return GL_READ_WRITE;
        default: break;
    }

    PN_CORE_ASSERT(false, "Unknown BufferMapAccess enum value");
    return 0;
}

}