#pragma once

#include <Peanut/Render/Buffers/IndexBuffer.hpp>

namespace pn {

class OpenGLIndexBuffer final : public IndexBuffer
{
public:
    OpenGLIndexBuffer(IndexBufferDataFormat format, uint32_t size, const void* data = nullptr);
    ~OpenGLIndexBuffer() override;

    void Bind() override;
    void Unbind() override;

    void ReplaceData(IndexBufferDataFormat format, const void* data, uint32_t size) override;
    void UpdateData(const void* data, uint32_t size, uint32_t offset = 0) override;

    uint32_t GetIndexCount() const override { return m_indexCount; }
    IndexBufferDataFormat GetDataFormat() const override { return m_format; }

private:
    void UpdateIndexCount();
    uint32_t GetGLDataTypeSize() const;

private:
    uint32_t m_handle;
    uint32_t m_size;
    uint32_t m_indexCount;
    IndexBufferDataFormat m_format;
};

}
