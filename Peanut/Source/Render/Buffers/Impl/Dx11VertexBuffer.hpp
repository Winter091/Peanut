#include <Peanut/Render/Buffers/VertexBuffer.hpp>

#include <d3d11.h>

namespace pn {

class Dx11VertexBuffer final : public VertexBuffer
{
public:
    Dx11VertexBuffer(BufferMapAccess access, uint32_t size, const void* data);
    ~Dx11VertexBuffer() override;

    void Bind() override;
    void BindToBindingIndex(uint32_t index) override;
    void Unbind() override;

    void* Map() override;
    void Unmap() override;

    const std::shared_ptr<BufferLayout>& GetLayout() const override;
    void SetLayout(const std::shared_ptr<BufferLayout>& layout) override;

    uint32_t GetSize() const { return m_size; }
    uint32_t GetVertexSize() const override;
    uint32_t GetVertexCount() const override;

    static std::shared_ptr<VertexBuffer> Create(BufferMapAccess access, uint32_t size, const void* data = nullptr);

private:
    ID3D11Buffer* m_handle = nullptr;
    uint32_t m_size;
    std::shared_ptr<BufferLayout> m_layout;
    bool m_isDataInitialized;
};

}