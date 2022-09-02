#include <Peanut/Render/Framebuffers/Framebuffer.hpp>

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>

#if defined(PN_RENDERING_DX11)
#include <Render/Framebuffers/Impl/Dx11Framebuffer.hpp>
#endif

namespace pn {

	std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSettings& settings, const std::string& name)
	{
        PN_CORE_ASSERT(settings.ColorTextures.size() <= 8, "Can't have more than 8 color textures in framebuffer");
        
        auto renderApi = RenderCommand::GetRenderAPI();

        switch (renderApi) {
            case RenderAPI::None:
                PN_CORE_ASSERT(false, "RenderAPI::None is not supported");
                break;

#if defined(PN_RENDERING_DX11)
            case RenderAPI::Dx11:
                return std::make_shared<Dx11Framebuffer>(settings, name);
#endif

            default:
                PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi));
                break;
        }

        return nullptr;
	}

}