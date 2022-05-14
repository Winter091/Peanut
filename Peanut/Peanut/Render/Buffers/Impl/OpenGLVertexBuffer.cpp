#include "OpenGLVertexBuffer.hpp"

#include <Peanut/Core/Assert.hpp>
#include <glad/glad.h>

namespace pn
{

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, const void* data)
    : m_size(size)
{
    PN_CORE_ASSERT(size > 0u, "Unable to create vertex buffer with size = 0");
    
    glCreateBuffers(1, &m_handle);
    glNamedBufferData(m_handle, size, data, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    glDeleteBuffers(1, &m_handle);
}

void OpenGLVertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
}

void OpenGLVertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0u);
}

void OpenGLVertexBuffer::ReplaceData(const void* data, uint32_t size)
{
    glNamedBufferData(m_handle, size, data, GL_STATIC_DRAW);
    m_size = size;
}

void OpenGLVertexBuffer::UpdateData(const void* data, uint32_t size, uint32_t offset)
{
    PN_CORE_ASSERT(
        offset + size <= m_size, 
        "Trying to update data out of bounds: last buffer index = {}, update range = ({}, {})",
        m_size - 1u, offset, offset + size - 1u
    );
    
    glNamedBufferSubData(m_handle, offset, size, data);
}

const BufferLayout& OpenGLVertexBuffer::GetLayout() const 
{
    PN_CORE_ASSERT(m_layout.GetStride() > 0u, "Layout is not specified");
    return m_layout;
}

}