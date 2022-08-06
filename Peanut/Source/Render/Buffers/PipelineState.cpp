#include <Peanut/Render/Buffers/PipelineState.hpp>

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>
#include <Render/Buffers/Impl/OpenGLPipelineState.hpp>
#include <Render/Buffers/Impl/Dx11PipelineState.hpp>

namespace pn
{

std::shared_ptr<PipelineState> PipelineState::Create(const PipelineStateDescription& description)
{
    auto renderApi = RenderCommand::GetRenderAPI();

    switch (renderApi) {
        case RenderAPI::None:
            PN_CORE_ASSERT(false, "RenderAPI::None is not supported"); 
            break;
        case RenderAPI::OpenGL:
            return std::make_shared<OpenGLPipelineState>(description);
        case RenderAPI::Dx11:
            return std::make_shared<Dx11PipelineState>(description);
        default:
            PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi)); 
            break;
    }

    return nullptr; 
}

}