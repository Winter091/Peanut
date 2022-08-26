#include "OpenGLBufferImpl.hpp"

#include <Peanut/Core/TimeProfiler.hpp>
#include "OpenGLEnumConversions.hpp"

#include <glad/glad.h>

namespace pn {

    OpenGLBufferImpl::OpenGLBufferImpl(BufferMapAccess access, uint32_t size, const void* data)
        : m_size(size)
        , m_mapAccess(access)
    {
        glCreateBuffers(1, &m_handle);
        glNamedBufferStorage(m_handle, size, data, BufferMapAccessToGlStorageAccess(access));
    }

    OpenGLBufferImpl::~OpenGLBufferImpl()
    {
        glDeleteBuffers(1, &m_handle);
    }

    void* OpenGLBufferImpl::Map()
    {
        PN_PROFILE_FUNCTION();

        PN_CORE_ASSERT(m_mapAccess != BufferMapAccess::NoAccess, "Trying to map buffer with no map access");

        void* buffer = glMapNamedBufferRange(m_handle, 0, m_size, BufferMapAccessToGlMapAccess(m_mapAccess));
        PN_CORE_ASSERT(buffer, "Mapped buffer is nullptr");
        return buffer;
    }

    void OpenGLBufferImpl::Unmap()
    {
        PN_PROFILE_FUNCTION();
        glUnmapNamedBuffer(m_handle);
    }

    void OpenGLBufferImpl::SetData(const void* data, size_t offset, size_t size)
    {
        PN_CORE_ASSERT(offset + size <= m_size, "Trying to set data outsize of buffer data range: buffer size = {}, "
                       "trying to set size = {} with offset = {}", m_size, size, offset);
    
        if (size == 0) {
            size = m_size - offset;
        }

        uint8_t* mappedData = reinterpret_cast<uint8_t*>(Map());
        memcpy(mappedData + offset, data, size);
        Unmap();
    }

}
