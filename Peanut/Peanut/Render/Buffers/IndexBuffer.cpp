#include "IndexBuffer.hpp"

#include <Peanut/Render/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>
#include <Peanut/Render/Buffers/Impl/OpenGLIndexBuffer.hpp>

#include <memory>

namespace pn
{

std::shared_ptr<IndexBuffer> IndexBuffer::Create(IndexBufferDataFormat format, uint32_t size, const void* data)
{
    auto renderApi = RenderCommand::GetRenderAPI();

    switch (renderApi) {
        case RenderAPI::None:
            PN_CORE_ASSERT(false, "RenderAPI::None is not supported"); 
            break;
        case RenderAPI::OpenGL:
            return std::make_shared<OpenGLIndexBuffer>(format, size, data);
        default:
            PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi)); 
            break;
    }

    return nullptr;
}

}