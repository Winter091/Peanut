#pragma once

#include <Render/Buffers/Impl/OpenGLEnumConversions.hpp>
#include <Peanut/Render/Buffers/IndexBuffer.hpp>
#include "OpenGLBufferImpl.hpp"

namespace pn {

    class OpenGLIndexBuffer final : public IndexBuffer, public OpenGLBufferImpl
    {
    public:
        OpenGLIndexBuffer(IndexBufferDataFormat format, BufferMapAccess access, uint32_t size, const void* data = nullptr)
            : OpenGLBufferImpl(access, size, data)
            , m_indexCount(size / GetIndexBufferDataFormatSize(format))
            , m_format(format)
        {}  

        ~OpenGLIndexBuffer() override = default;

        void* Map() override { return OpenGLBufferImpl::Map(); }
        void Unmap() override { OpenGLBufferImpl::Unmap(); }
        void SetData(const void* data, size_t offset = 0, size_t size = 0) override { OpenGLBufferImpl::SetData(data, offset, size); }

        uint32_t GetSize() const override { return OpenGLBufferImpl::GetSize(); }
        uint32_t GetIndexCount() const override { return m_indexCount; }
        IndexBufferDataFormat GetDataFormat() const override { return m_format; }

    private:
        uint32_t m_indexCount;
        IndexBufferDataFormat m_format;
    };

}
