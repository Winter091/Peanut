#ifndef __DX11VERTEXARRAY_H__
#define __DX11VERTEXARRAY_H__

#include <Peanut/Render/Buffers/VertexArray.hpp>
#include <dxgiformat.h>

class ID3D11InputLayout;
class D3D11_INPUT_ELEMENT_DESC;

namespace pn {

class DX11VertexArray final : public VertexArray
{
public:
    DX11VertexArray();
    ~DX11VertexArray() override;

    void Bind() override;
    void Unbind() override;

    void SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
    void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

    bool GetIsVertexBufferInitialized() const override;
    bool GetIsIndexBufferInitialized() const override;

    uint32_t GetSize() const override;
    uint32_t GetVertexCount() const override;
    uint32_t GetIndexCount() const override;
    IndexBufferDataFormat GetIndexDataFormat() const override;

private:
    ID3D11InputLayout* m_vertexLayout = nullptr;
    std::shared_ptr<VertexBuffer> m_vertexBuffer;
    std::shared_ptr<IndexBuffer> m_indexBuffer;

private:
    void ProcessVertexBufferLayout();
    D3D11_INPUT_ELEMENT_DESC ProcessVertexBufferLayoutElement(
        const BufferLayoutElement& elem, uint32_t index, uint32_t stride);
    DXGI_FORMAT MapTypeToDXFormat(BufferLayoutElementType type) const;
};

}

#endif