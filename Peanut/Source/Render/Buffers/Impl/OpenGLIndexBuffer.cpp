#include "OpenGLIndexBuffer.hpp"

#include <Peanut/Core/Assert.hpp>
#include <glad/glad.h>

namespace pn
{

OpenGLIndexBuffer::OpenGLIndexBuffer(IndexBufferDataFormat format, uint32_t size, const void* data)
    : m_size(size)
    , m_indexCount(0u)
    , m_format(format)
    , m_isDataInitialized(false)
{
    PN_CORE_ASSERT(size > 0u, "Unable to create vertex buffer with size = 0");

    glCreateBuffers(1, &m_handle);
    ReplaceData(format, data, size);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    glDeleteBuffers(1, &m_handle);
}

void OpenGLIndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
}

void OpenGLIndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
}

void OpenGLIndexBuffer::ReplaceData(IndexBufferDataFormat format, const void* data, uint32_t size)
{
    PN_CORE_ASSERT(size > 0u, "Unable to create vertex buffer with size = 0");
    
    glNamedBufferData(m_handle, size, data, GL_STATIC_DRAW);

    m_size = size;
    m_format = format;
    m_isDataInitialized = (data != nullptr);
    UpdateIndexCount();
}

void OpenGLIndexBuffer::UpdateData(const void* data, uint32_t size, uint32_t offset)
{
    PN_CORE_ASSERT(data, "Unable to replace data, nullptr is provided");
    PN_CORE_ASSERT(
        offset + size <= m_size, 
        "Trying to update data out of bounds: last buffer index = {}, update range = ({}, {})",
        m_size - 1u, offset, offset + size - 1u
    );
    
    glNamedBufferSubData(m_handle, offset, size, data);
    m_isDataInitialized = true;
}

uint32_t OpenGLIndexBuffer::GetGLDataTypeSize() const
{
    switch (m_format) {
        case IndexBufferDataFormat::Uint8:  return 1u;
        case IndexBufferDataFormat::Uint16: return 2u;
        case IndexBufferDataFormat::Uint32: return 4u;
        default: break;
    }

    PN_CORE_ASSERT(false, "Unknown index buffer data format: {}", static_cast<uint32_t>(m_format));
    return 0u;
}

void OpenGLIndexBuffer::UpdateIndexCount()
{
    m_indexCount = m_size / GetGLDataTypeSize();
}

}