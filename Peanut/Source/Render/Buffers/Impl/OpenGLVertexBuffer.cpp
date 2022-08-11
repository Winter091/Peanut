#include "OpenGLVertexBuffer.hpp"

#include <Peanut/Core/TimeProfiler.hpp>
#include <Render/Buffers/Impl/OpenGLEnumConversions.hpp>

#include <glad/glad.h>

namespace pn
{

OpenGLVertexBuffer::OpenGLVertexBuffer(BufferMapAccess access, uint32_t size, const std::shared_ptr<BufferLayout>& layout, const void* data)
    : m_handle(0)
    , m_size(size)
    , m_mapAccess(access)
    , m_layout(layout)
{
    PN_PROFILE_FUNCTION();

    PN_CORE_ASSERT(m_size > 0u, "Unable to create buffer with size = 0");
    
    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, size, data, BufferMapAccessToGlStorageAccess(access));
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    glDeleteBuffers(1, &m_handle);
}

void* OpenGLVertexBuffer::Map()
{
    PN_PROFILE_FUNCTION();

    PN_CORE_ASSERT(m_mapAccess != BufferMapAccess::NoAccess, "Trying to map buffer with no map access");

    void* buffer = glMapNamedBufferRange(m_handle, 0, m_size, BufferMapAccessToGlMapAccess(m_mapAccess));
    PN_CORE_ASSERT(buffer, "Mapped buffer is nullptr");
    return buffer;
}

void OpenGLVertexBuffer::Unmap()
{
    PN_PROFILE_FUNCTION();

    glUnmapNamedBuffer(m_handle);
}

const std::shared_ptr<BufferLayout>& OpenGLVertexBuffer::GetLayout() const 
{
    PN_CORE_ASSERT(m_layout, "Layout is not specified");
    return m_layout;
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