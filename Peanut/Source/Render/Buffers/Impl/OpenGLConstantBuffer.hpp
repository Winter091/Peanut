#pragma once

#include <Peanut/Render/Buffers/ConstantBuffer.hpp>
#include "OpenGLBufferImpl.hpp"

namespace pn {

    class OpenGLConstantBuffer final : public ConstantBuffer, public OpenGLBufferImpl
    {
    public:
        OpenGLConstantBuffer(BufferMapAccess access, uint32_t size, const void* data)
            : OpenGLBufferImpl(access, size, data)
        {}

        ~OpenGLConstantBuffer() override = default;

        void* Map() override { return OpenGLBufferImpl::Map(); }
        void Unmap() override { OpenGLBufferImpl::Unmap(); }
        void SetData(const void* data, size_t offset = 0, size_t size = 0) override { OpenGLBufferImpl::SetData(data, offset, size); }

        uint32_t GetSize() const override { return OpenGLBufferImpl::GetSize(); }
        uint32_t GetOpenGLHandle() const { return OpenGLBufferImpl::GetOpenGLHandle(); }
    };

}
