#ifndef __OPENGLVERTEXBUFFER_H__
#define __OPENGLVERTEXBUFFER_H__

#include <Peanut/Render/Buffers/VertexBuffer.hpp>

namespace pn {

class OpenGLVertexBuffer final : public VertexBuffer
{
public:
    OpenGLVertexBuffer(uint32_t size, const void* data = nullptr);
    ~OpenGLVertexBuffer() override;

    void Bind() override;
    void Unbind() override;
    
    void SetData(const void* data, uint32_t size) override;
    
    const BufferLayout& GetLayout() const override;
    void SetLayout(const BufferLayout& layout) override;

private:

};

}

#endif