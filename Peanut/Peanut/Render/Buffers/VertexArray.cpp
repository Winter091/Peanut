#include "VertexArray.hpp"

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>

#include <Peanut/Render/Buffers/Impl/DX11VertexArray.hpp>
#include <Peanut/Render/Buffers/Impl/OpenGLVertexArray.hpp>

namespace pn
{

std::shared_ptr<VertexArray> VertexArray::Create()
{
    auto renderApi = RenderCommand::GetRenderAPI();

    switch (renderApi) {
        case RenderAPI::None:
            PN_CORE_ASSERT(false, "RenderAPI::None is not supported"); 
            break;
        case RenderAPI::OpenGL:
            return std::make_shared<OpenGLVertexArray>();
        case RenderAPI::DX11:
            return std::make_shared<DX11VertexArray>();
        default:
            PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi)); 
            break;
    }

    return nullptr; 
}

}