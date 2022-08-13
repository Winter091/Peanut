#include <Peanut/Render/Buffers/PipelineState.hpp>

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>

#if defined(PN_RENDERING_OPENGL)
#include <Render/Buffers/Impl/OpenGLPipelineState.hpp>
#endif

#if defined(PN_RENDERING_DX11)
#include <Render/Buffers/Impl/Dx11PipelineState.hpp>
#endif

namespace pn
{

std::shared_ptr<PipelineState> PipelineState::Create(const PipelineStateDescription& description)
{
    auto renderApi = RenderCommand::GetRenderAPI();

    switch (renderApi) {
        case RenderAPI::None:
            PN_CORE_ASSERT(false, "RenderAPI::None is not supported"); 
            break;

#if defined(PN_RENDERING_OPENGL)
        case RenderAPI::OpenGL:
            return std::make_shared<OpenGLPipelineState>(description);
#endif

#if defined(PN_RENDERING_DX11)
        case RenderAPI::Dx11:
            return std::make_shared<Dx11PipelineState>(description);
#endif

        default:
            PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi)); 
            break;
    }


    return nullptr; 
}

}