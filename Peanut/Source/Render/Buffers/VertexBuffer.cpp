#include <Peanut/Render/Buffers/VertexBuffer.hpp>

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>

#include <Render/Buffers/Impl/OpenGLVertexBuffer.hpp>

namespace pn {

std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size, const void* data, BufferDataUsage usage)
{
    auto renderApi = RenderCommand::GetRenderAPI();

    switch (renderApi) {
        case RenderAPI::None:
            PN_CORE_ASSERT(false, "RenderAPI::None is not supported"); 
            break;
        case RenderAPI::OpenGL:
            return std::make_shared<OpenGLVertexBuffer>(size, data, usage);
        default:
            PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi)); 
            break;
    }

    return nullptr;
}

}