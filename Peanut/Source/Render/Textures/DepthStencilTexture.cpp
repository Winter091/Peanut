#include <Peanut/Render/Textures/DepthStencilTexture.hpp>

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>

#if defined(PN_RENDERING_DX11)
#include <Render/Textures/Impl/Dx11DepthStencilTexture.hpp>
#endif

namespace pn {

	std::shared_ptr<DepthStencilTexture> DepthStencilTexture::Create(const DepthStencilTextureSettings& settings, const std::string& name)
	{
		PN_CORE_ASSERT(settings.Size.x > 0, "Unable to create depth stencil texture with width = 0");
		PN_CORE_ASSERT(settings.Size.y > 0, "Unable to create depth stencil texture with height = 0");

        auto renderApi = RenderCommand::GetRenderAPI();

        switch (renderApi) {
            case RenderAPI::None:
                PN_CORE_ASSERT(false, "RenderAPI::None is not supported");
                break;

#if defined(PN_RENDERING_DX11)
            case RenderAPI::Dx11:
                return std::make_shared<Dx11DepthStencilTexture>(settings, name);
#endif

            default:
                PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi));
                break;
        }

        return nullptr;
	}

}
