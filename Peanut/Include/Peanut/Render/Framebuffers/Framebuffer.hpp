#pragma once

#include <Peanut/Render/Textures/Texture2D.hpp>
#include <Peanut/Render/Textures/DepthStencilTexture.hpp>

#include <vector>
#include <memory>

namespace pn {

	struct FramebufferSettings
	{
	public:
		FramebufferSettings& AddColorTexture(const std::shared_ptr<Texture2D>& texture) { ColorTextures.push_back(texture); return *this; }
		FramebufferSettings& SetColorTextures(const std::vector<std::shared_ptr<Texture2D>> textures) { ColorTextures = textures; return *this; }
		FramebufferSettings& SetDepthStencilTexture(const std::shared_ptr<DepthStencilTexture>& texture) { DepthStencilTexture= texture; return *this; }

	public:
		std::vector<std::shared_ptr<Texture2D>> ColorTextures;
		std::shared_ptr<DepthStencilTexture> DepthStencilTexture;
	};

	class Framebuffer
	{
	public:
		Framebuffer() = default;
		Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator=(const Framebuffer&) = delete;
		virtual ~Framebuffer() = default;

		virtual const std::vector<std::shared_ptr<Texture2D>>& GetColorTextures() const = 0;

		static std::shared_ptr<Framebuffer> Create(const FramebufferSettings& settings, const std::string& name = "");

		static inline const std::shared_ptr<Framebuffer> Default = nullptr;
	};

}
