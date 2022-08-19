#include <Peanut/Render/Textures/TextureSampler.hpp>

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>

#if defined(PN_RENDERING_OPENGL)
#include <Render/Textures/Impl/OpenGLTextureSampler.hpp>
#endif

namespace pn {

    std::shared_ptr<TextureSampler> TextureSampler::Create(const TextureSamplerSettings& settings)
    {
        auto renderApi = RenderCommand::GetRenderAPI();

        switch (renderApi) {
            case RenderAPI::None:
                PN_CORE_ASSERT(false, "RenderAPI::None is not supported");
                break;

#if defined(PN_RENDERING_OPENGL)
            case RenderAPI::OpenGL:
                return std::make_shared<OpenGLTextureSampler>(settings);
#endif

            default:
                PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi));
                break;
        }

        return nullptr;
    }

}
