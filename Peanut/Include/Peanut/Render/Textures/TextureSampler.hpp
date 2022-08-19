#pragma once

#include <memory>

namespace pn {

	enum class TextureFilter
	{
		Nearest,
		Linear,
		Anisotropic,
	};

	enum class TextureMipFilter
	{
		Nearest,
		Linear,
	};

	enum class TextureWrap
	{
		Repeat,
		MirroredRepeat,
		Clamp,
		Border,
	};

	struct TextureSamplerSettings
	{
	public:
		TextureSamplerSettings& SetMinFilter(TextureFilter minFilter)
		{
			m_minFilter = minFilter;
			return *this;
		}

		TextureSamplerSettings& SetMagFilter(TextureFilter magFilter)
		{
			m_magFilter = magFilter;
			return *this;
		}

		TextureSamplerSettings& SetMipFilter(TextureMipFilter mipFilter)
		{
			m_mipFilter = mipFilter;
			return *this;
		}

		TextureSamplerSettings& SetWrap(TextureWrap wrap)
		{
			m_wrapX = wrap;
			m_wrapY = wrap;
			m_wrapZ = wrap;
			return *this;
		}

		TextureSamplerSettings& SetWrapX(TextureWrap wrap)
		{
			m_wrapX = wrap;
			return *this;
		}

		TextureSamplerSettings& SetWrapY(TextureWrap wrap)
		{
			m_wrapY = wrap;
			return *this;
		}

		TextureSamplerSettings& SetWrapZ(TextureWrap wrap)
		{
			m_wrapZ = wrap;
			return *this;
		}

		TextureFilter GetMinFilter() const { return m_minFilter; }
		TextureFilter GetMagFilter() const { return m_magFilter; }
		TextureMipFilter GetMipFilter() const { return m_mipFilter; }
		TextureWrap GetWrapX() const { return m_wrapX; }
		TextureWrap GetWrapY() const { return m_wrapY; }
		TextureWrap GetWrapZ() const { return m_wrapZ; }

	private:
		TextureFilter m_minFilter = TextureFilter::Anisotropic;
		TextureFilter m_magFilter = TextureFilter::Anisotropic;
		TextureMipFilter m_mipFilter = TextureMipFilter::Linear;
		TextureWrap m_wrapX = TextureWrap::Repeat;
		TextureWrap m_wrapY = TextureWrap::Repeat;
		TextureWrap m_wrapZ = TextureWrap::Repeat;
	};

	class TextureSampler
	{
	public:
		virtual ~TextureSampler() = default;

		static std::shared_ptr<TextureSampler> Create(const TextureSamplerSettings& settings);
	};

}
