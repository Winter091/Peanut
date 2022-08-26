#pragma once

#include <Peanut/Render/Textures/TextureSampler.hpp>

namespace pn {

	class OpenGLTextureSampler final : public TextureSampler
	{
	public:
		OpenGLTextureSampler(const TextureSamplerSettings& settings);
		~OpenGLTextureSampler() override;

		uint32_t GetOpenGLHandle() const { return m_handle; }

	private:
		uint32_t m_handle = 0;
	};

}
