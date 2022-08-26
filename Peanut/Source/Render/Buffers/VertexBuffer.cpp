#include <Peanut/Render/Buffers/VertexBuffer.hpp>

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>

#if defined(PN_RENDERING_OPENGL)
#include <Render/Buffers/Impl/OpenGLVertexBuffer.hpp>
#endif

#if defined(PN_RENDERING_DX11)
#include <Render/Buffers/Impl/Dx11VertexBuffer.hpp>
#endif

namespace pn {

    std::shared_ptr<VertexBuffer> VertexBuffer::Create(
        const std::shared_ptr<BufferLayout>& layout, BufferMapAccess access,
        VertexBufferDataUsage dataUsage, uint32_t size, const void* data)
    {
        PN_CORE_ASSERT(layout, "Buffer layout cannot be nullptr");
        PN_CORE_ASSERT(size % layout->GetVertexSize() == 0, "Vertex buffer size is not a multiple of vertex size");
    
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
                return std::make_shared<OpenGLVertexBuffer>(layout, access, dataUsage, size, data);
#endif

#if defined(PN_RENDERING_DX11)
            case RenderAPI::Dx11:
                return std::make_shared<Dx11VertexBuffer>(layout, access, dataUsage, size, data);
#endif

            default:
                PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi)); 
                break;
        }


        return nullptr;
    }

}
