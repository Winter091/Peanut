#include "Dx11Framebuffer.hpp"

#include <Render/Textures/Impl/Dx11Texture2D.hpp>
#include <Render/Textures/Impl/Dx11DepthStencilTexture.hpp>

namespace pn {

	Dx11Framebuffer::Dx11Framebuffer(const FramebufferSettings& settings, const std::string& name)
		: m_name(name)
		, m_colorTextures(settings.ColorTextures)
		, m_cachedRenderTargetViewPointers(settings.ColorTextures.size(), nullptr)
		, m_depthStencilTexture(settings.DepthStencilTexture)
	{
		for (size_t i = 0; i < m_colorTextures.size(); i++) {
			m_cachedRenderTargetViewPointers[i] = dynamic_cast<Dx11Texture2D&>(*m_colorTextures[i]).GetNativeRenderTargetViewPtr();
		}

		if (!m_cachedRenderTargetViewPointers.empty()) {
			m_cachedRenderTargetViewRawPointers = m_cachedRenderTargetViewPointers.data();
		}

		if (m_depthStencilTexture) {
			m_cachedDepthStencilViewRawPointer = static_cast<Dx11DepthStencilTexture&>(*m_depthStencilTexture).GetNativeDepthStencilView();
		}
	}

}
