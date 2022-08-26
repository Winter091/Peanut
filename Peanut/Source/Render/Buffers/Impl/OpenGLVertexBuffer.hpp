#pragma once

#include <Peanut/Render/Buffers/VertexBuffer.hpp>
#include "OpenGLBufferImpl.hpp"

namespace pn {

    class OpenGLVertexBuffer final : public VertexBuffer, public OpenGLBufferImpl
    {
    public:
        OpenGLVertexBuffer(const std::shared_ptr<BufferLayout>& layout, BufferMapAccess access,
                           VertexBufferDataUsage dataUsage, uint32_t size, const void* data)
            : OpenGLBufferImpl(access, size, data)
            , m_dataUsage(dataUsage)
            , m_layout(layout)
        {}

        ~OpenGLVertexBuffer() override = default;

        void* Map() override { return OpenGLBufferImpl::Map(); }
        void Unmap() override { OpenGLBufferImpl::Unmap(); }
        void SetData(const void* data, size_t offset = 0, size_t size = 0) { OpenGLBufferImpl::SetData(data, offset, size); }

        const std::shared_ptr<BufferLayout>& GetLayout() const override { return m_layout; }

        uint32_t GetSize() const override { return OpenGLBufferImpl::GetSize(); }
        uint32_t GetVertexSize() const override { return m_layout->GetVertexSize(); }
        uint32_t GetVertexCount() const override { return OpenGLBufferImpl::GetSize() / GetVertexSize(); }
        VertexBufferDataUsage GetDataUsage() const override { return m_dataUsage; }

    private:
        VertexBufferDataUsage m_dataUsage;
        std::shared_ptr<BufferLayout> m_layout;
    };

}
