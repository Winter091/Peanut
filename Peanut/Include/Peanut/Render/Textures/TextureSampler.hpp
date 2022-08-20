#pragma once

#include <Peanut/Core/Assert.hpp>

#include <glm/vec4.hpp>

#include <memory>

namespace pn {

	enum class TextureFilter : uint32_t
	{
		MinPointMagPointMipPoint = 0u,
		MinPointMagPointMipLinear,
		MinPointMagLinearMipPoint,
		MinPointMagLinearMipLinear,
		MinLinearMagPointMipPoint,
		MinLinearMagPointMipLinear,
		MinLinearMagLinearMipPoint,
		MinLinearMagLinearMipLinear,
		MinAnisotropicMagAnisotropicMipAnisotropic,
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
		TextureSamplerSettings& SetFilter(TextureFilter filter)
		{
			m_filter = filter;
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

		TextureSamplerSettings& SetMaxAnisotropy(float value) {
			PN_CORE_ASSERT(value >= 1.0f && value <= 16.0f, "Max anosotropy value has to be between 1.0 and 16.0");
			m_maxAnisotropy = value;
			return *this;
		}

		TextureSamplerSettings& SetBorderColor(const glm::vec4& color) {
			m_borderColor = color;
			return *this;
		}

		TextureSamplerSettings& SetLodBias(float bias) {
			m_lodBias = bias;
			return *this;
		}

		TextureSamplerSettings& SetMinLod(float lod) {
			m_minLod = lod;
			return *this;
		}

		TextureSamplerSettings& SetMaxLod(float lod) {
			m_maxLod = lod;
			return *this;
		}

		TextureFilter GetFilter() const { return m_filter; }
		TextureWrap GetWrapX() const { return m_wrapX; }
		TextureWrap GetWrapY() const { return m_wrapY; }
		TextureWrap GetWrapZ() const { return m_wrapZ; }
		float GetMaxAnisotropy() const { return m_maxAnisotropy; }
		const glm::vec4& GetBorderColor() const { return m_borderColor; }
		float GetLodBias() const { return m_lodBias; }
		float GetMinLod() const { return m_minLod; }
		float GetMaxLod() const { return m_maxLod; }

	private:
		TextureFilter m_filter = TextureFilter::MinLinearMagLinearMipLinear;
		TextureWrap m_wrapX = TextureWrap::Repeat;
		TextureWrap m_wrapY = TextureWrap::Repeat;
		TextureWrap m_wrapZ = TextureWrap::Repeat;
		float m_maxAnisotropy = 1.0f;
		glm::vec4 m_borderColor = glm::vec4(0.0f);
		float m_lodBias = 0.0f;
		float m_minLod = 0.0f;
		float m_maxLod = std::numeric_limits<float>::max();
	};

	class TextureSampler
	{
	public:
		virtual ~TextureSampler() = default;

		static std::shared_ptr<TextureSampler> Create(const TextureSamplerSettings& settings);
	};

}
