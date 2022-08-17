#include "OpenGLBuffer.hpp"

#include <Peanut/Core/TimeProfiler.hpp>
#include "OpenGLEnumConversions.hpp"

#include <glad/glad.h>

namespace pn {

OpenGLBuffer::OpenGLBuffer(BufferMapAccess access, uint32_t size, const void* data)
    : m_size(size)
    , m_mapAccess(access)
{
    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, size, data, BufferMapAccessToGlStorageAccess(access));
}

OpenGLBuffer::~OpenGLBuffer()
{
    glDeleteBuffers(1, &m_handle);
}

void* OpenGLBuffer::Map()
{
    PN_PROFILE_FUNCTION();

    PN_CORE_ASSERT(m_mapAccess != BufferMapAccess::NoAccess, "Trying to map buffer with no map access");

    void* buffer = glMapNamedBufferRange(m_handle, 0, m_size, BufferMapAccessToGlMapAccess(m_mapAccess));
    PN_CORE_ASSERT(buffer, "Mapped buffer is nullptr");
    return buffer;
}

void OpenGLBuffer::Unmap()
{
    PN_PROFILE_FUNCTION();
    glUnmapNamedBuffer(m_handle);
}

}