#include <Peanut/Render/Buffers/VertexBuffer.hpp>

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>

#include <Render/Buffers/Impl/Dx11VertexBuffer.hpp>
#include <Render/Buffers/Impl/OpenGLVertexBuffer.hpp>

namespace pn {

std::shared_ptr<VertexBuffer> VertexBuffer::Create(BufferMapAccess access, uint32_t size, const std::shared_ptr<BufferLayout>& layout, const void* data)
{
    if (!data) {
        PN_CORE_ASSERT(DoesBufferMapAccessAllowWriting(access), "Data for vertex buffer without write permissions must be specified on creation");
    }

    auto renderApi = RenderCommand::GetRenderAPI();

    switch (renderApi) {
        case RenderAPI::None:
            PN_CORE_ASSERT(false, "RenderAPI::None is not supported"); 
            break;

#if defined(PN_RENDERING_OPENGL)
        case RenderAPI::OpenGL:
            return std::make_shared<OpenGLVertexBuffer>(access, size, layout, data);
#endif

#if defined(PN_RENDERING_DX11)
        case RenderAPI::Dx11:
            return std::make_shared<Dx11VertexBuffer>(access, size, layout, data);
#endif

        default:
            PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi)); 
            break;
    }


    return nullptr;
}

}