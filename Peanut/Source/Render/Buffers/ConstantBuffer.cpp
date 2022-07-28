#include <Peanut/Render/Buffers/ConstantBuffer.hpp>

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>

#include <Render/Buffers/Impl/OpenGLConstantBuffer.hpp>

namespace pn {

std::shared_ptr<ConstantBuffer> ConstantBuffer::Create(BufferMapAccess access, uint32_t size, const void* data)
{
    auto renderApi = RenderCommand::GetRenderAPI();

    switch (renderApi) {
        case RenderAPI::None:
            PN_CORE_ASSERT(false, "RenderAPI::None is not supported"); 
            break;
        case RenderAPI::OpenGL:
            return std::make_shared<OpenGLConstantBuffer>(access, size, data);
        default:
            PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi)); 
            break;
    }

    return nullptr;
}

}