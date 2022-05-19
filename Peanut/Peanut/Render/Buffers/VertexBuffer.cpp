#include "VertexBuffer.hpp"

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>

#include <Peanut/Render/Buffers/Impl/OpenGLVertexBuffer.hpp>
#include <Peanut/Render/Buffers/Impl/DX11VertexBuffer.hpp>

namespace pn {

std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size, const void* data)
{
    auto renderApi = RenderCommand::GetRenderAPI();

    switch (renderApi) {
        case RenderAPI::None:
            PN_CORE_ASSERT(false, "RenderAPI::None is not supported"); 
            break;
        case RenderAPI::OpenGL:
            return std::make_shared<OpenGLVertexBuffer>(size, data);
        case RenderAPI::DX11:
            return std::make_shared<DX11VertexBuffer>(size, data);
        default:
            PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi)); 
            break;
    }

    return nullptr;
}

}