#include <Peanut/Render/Buffers/IndexBuffer.hpp>

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>
#include <Render/Buffers/Impl/OpenGLIndexBuffer.hpp>
#include <Render/Buffers/Impl/Dx11IndexBuffer.hpp>

#include <memory>

namespace pn
{

std::shared_ptr<IndexBuffer> IndexBuffer::Create(IndexBufferDataFormat format, BufferMapAccess access, uint32_t size, const void* data)
{
    if (!data) {
        PN_CORE_ASSERT(DoesBufferMapAccessAllowWriting(access), "Data for index buffer without write permissions must be specified on creation");
    }
    
    auto renderApi = RenderCommand::GetRenderAPI();

    switch (renderApi) {
        case RenderAPI::None:
            PN_CORE_ASSERT(false, "RenderAPI::None is not supported"); 
            break;
        case RenderAPI::OpenGL:
            return std::make_shared<OpenGLIndexBuffer>(format, access, size, data);
        case RenderAPI::Dx11:
            return std::make_shared<Dx11IndexBuffer>(format, access, size, data);
        default:
            PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi)); 
            break;
    }

    return nullptr;
}

}