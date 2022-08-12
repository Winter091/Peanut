#include <Peanut/Render/Buffers/ConstantBuffer.hpp>

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>

#include <Render/Buffers/Impl/Dx11ConstantBuffer.hpp>
#include <Render/Buffers/Impl/OpenGLConstantBuffer.hpp>

namespace pn {

std::shared_ptr<ConstantBuffer> ConstantBuffer::Create(BufferMapAccess access, uint32_t size, const void* data)
{
    PN_CORE_ASSERT(size > 0, "Unable to create constant buffer with size = 0");
    PN_CORE_ASSERT(size % 16 == 0, "Constant buffer size has to be multiple of 16");
    PN_CORE_ASSERT(access != BufferMapAccess::Read && access != BufferMapAccess::ReadWrite, "Constant buffer cannot be mapped with read permissions");

    if (!data) {
        PN_CORE_ASSERT(DoesBufferMapAccessAllowWriting(access), "Data for constant buffer without write permissions must be specified on creation");
    }

    auto renderApi = RenderCommand::GetRenderAPI();

    switch (renderApi) {
        case RenderAPI::None:
            PN_CORE_ASSERT(false, "RenderAPI::None is not supported"); 
            break;
        case RenderAPI::OpenGL:
            return std::make_shared<OpenGLConstantBuffer>(access, size, data);
        case RenderAPI::Dx11:
            return std::make_shared<Dx11ConstantBuffer>(access, size, data);
        default:
            PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi)); 
            break;
    }

    return nullptr;
}

}