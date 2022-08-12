#include "OpenGLConstantBuffer.hpp"

#include <Peanut/Core/TimeProfiler.hpp>
#include <Render/Buffers/Impl/OpenGLEnumConversions.hpp>

#include <glad/glad.h>

namespace pn
{

OpenGLConstantBuffer::OpenGLConstantBuffer(BufferMapAccess access, uint32_t size, const void* data)
    : m_handle(0)
    , m_size(size)
    , m_mapAccess(access)
{
    PN_PROFILE_FUNCTION();

    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, size, data, BufferMapAccessToGlStorageAccess(access));
}

OpenGLConstantBuffer::~OpenGLConstantBuffer()
{
    glDeleteBuffers(1, &m_handle);
}

void* OpenGLConstantBuffer::Map()
{
    PN_PROFILE_FUNCTION();

    PN_CORE_ASSERT(m_mapAccess != BufferMapAccess::NoAccess, "Trying to map buffer with no map access");

    void* buffer = glMapNamedBufferRange(m_handle, 0, m_size, BufferMapAccessToGlMapAccess(m_mapAccess));
    PN_CORE_ASSERT(buffer, "Mapped buffer is nullptr");
    return buffer;
}

void OpenGLConstantBuffer::Unmap()
{
    PN_PROFILE_FUNCTION();

    glUnmapNamedBuffer(m_handle);
}

}