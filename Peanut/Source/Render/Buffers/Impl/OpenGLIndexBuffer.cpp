#include "OpenGLIndexBuffer.hpp"

#include <Peanut/Core/TimeProfiler.hpp>
#include <Render/Buffers/Impl/OpenGLEnumConversions.hpp>

namespace pn
{

OpenGLIndexBuffer::OpenGLIndexBuffer(IndexBufferDataFormat format, BufferMapAccess access, uint32_t size, const void* data)
    : m_handle(0)
    , m_size(size)
    , m_indexCount(0u)
    , m_mapAccess(access)
    , m_format(format)
{
    PN_PROFILE_FUNCTION();

    PN_CORE_ASSERT(size > 0u, "Unable to create vertex buffer with size = 0");

    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, size, data, BufferMapAccessToGlStorageAccess(access));
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

void* OpenGLIndexBuffer::Map()
{
    PN_PROFILE_FUNCTION();

    PN_CORE_ASSERT(m_mapAccess != BufferMapAccess::NoAccess, "Trying to map buffer with no map access");

    void* buffer = glMapNamedBuffer(m_handle, BufferMapAccessToGlMapAccess(m_mapAccess));
    PN_CORE_ASSERT(buffer, "Mapped buffer is nullptr");
    return buffer;
}

void OpenGLIndexBuffer::Unmap()
{
    PN_PROFILE_FUNCTION();

    glUnmapNamedBuffer(m_handle);
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