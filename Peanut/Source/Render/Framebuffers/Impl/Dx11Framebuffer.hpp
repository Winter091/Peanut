#pragma once

#include <Peanut/Render/Framebuffers/Framebuffer.hpp>

struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace pn {

	class Dx11Framebuffer : public Framebuffer
	{
	public:
		Dx11Framebuffer(const FramebufferSettings& settings, const std::string& name);
		~Dx11Framebuffer() override = default;

		const std::vector<std::shared_ptr<Texture2D>>& GetColorTextures() const override { return m_colorTextures; }

		uint32_t GetNumRenderTargetViews() const { return static_cast<uint32_t>(m_colorTextures.size()); }
		ID3D11RenderTargetView** GetRenderTargetViewNativePointers() const { return m_cachedRenderTargetViewRawPointers; }
		ID3D11DepthStencilView* GetDepthStencilViewNativePointer() const { return m_cachedDepthStencilViewRawPointer; }

	private:
		std::string m_name;
		std::vector<std::shared_ptr<Texture2D>> m_colorTextures;
		std::vector<ID3D11RenderTargetView*> m_cachedRenderTargetViewPointers;
		ID3D11RenderTargetView** m_cachedRenderTargetViewRawPointers;

		std::shared_ptr<DepthStencilTexture> m_depthStencilTexture;
		ID3D11DepthStencilView* m_cachedDepthStencilViewRawPointer;

	};

}
