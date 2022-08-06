#include <Peanut/Render/Buffers/VertexArray.hpp>

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>
#include <Render/Buffers/Impl/OpenGLVertexArray.hpp>
#include <Render/Buffers/Impl/Dx11PipelineState.hpp>

namespace pn
{

std::shared_ptr<VertexArray> VertexArray::Create(const VertexArrayDescription& description)
{
    auto renderApi = RenderCommand::GetRenderAPI();

    switch (renderApi) {
        case RenderAPI::None:
            PN_CORE_ASSERT(false, "RenderAPI::None is not supported"); 
            break;
        case RenderAPI::OpenGL:
            return std::make_shared<OpenGLVertexArray>(description);
        case RenderAPI::Dx11:
            return std::make_shared<Dx11PipelineState>(description);
        default:
            PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi)); 
            break;
    }

    return nullptr; 
}

}