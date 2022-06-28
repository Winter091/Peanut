#include "OpenGLVertexBuffer.hpp"

#include <Peanut/Core/Assert.hpp>
#include <glad/glad.h>

namespace pn
{

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, const void* data)
{
    PN_CORE_ASSERT(size > 0u, "Unable to create vertex buffer with size = 0");
    
    glCreateBuffers(1, &m_handle);
    ReplaceData(size, data);
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

void OpenGLVertexBuffer::ReplaceData(uint32_t size, const void* data)
{
    glNamedBufferData(m_handle, size, data, GL_STATIC_DRAW);

    m_size = size;
    m_isDataInitialized = (data != nullptr);
}

void OpenGLVertexBuffer::UpdateData(uint32_t size, const void* data, uint32_t offset)
{
    PN_CORE_ASSERT(data, "Data provided for UpdateData is nullptr");
    PN_CORE_ASSERT(
        offset + size <= m_size, 
        "Trying to update data out of bounds: last buffer index = {}, update range = ({}, {})",
        m_size - 1u, offset, offset + size - 1u
    );
    
    glNamedBufferSubData(m_handle, offset, size, data);
    m_isDataInitialized = true;
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

}