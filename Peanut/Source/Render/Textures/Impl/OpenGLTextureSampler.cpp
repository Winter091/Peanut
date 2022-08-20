#include "OpenGLTextureSampler.hpp"

#include "OpenGLEnumConversions.hpp"

namespace pn {

	OpenGLTextureSampler::OpenGLTextureSampler(const TextureSamplerSettings& settings)
	{
		glGenSamplers(1, &m_handle);

		glSamplerParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GetGlMinFilter(settings.GetFilter()));
		glSamplerParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GetGlMagFilter(settings.GetFilter()));

		glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_S, GetGLWrap(settings.GetWrapX()));
		glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_T, GetGLWrap(settings.GetWrapY()));

		if (settings.GetFilter() != TextureFilter::MinAnisotropicMagAnisotropicMipAnisotropic) {
			glSamplerParameterf(m_handle, GL_TEXTURE_MAX_ANISOTROPY, 1.0f);
		} else {
			float maxSupportedAnisotropy = 0.0f;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxSupportedAnisotropy);

			if (settings.GetMaxAnisotropy() > maxSupportedAnisotropy) {
				PN_CORE_WARN("Texture sampler: specified max anisotropy value {} is clamped to {} because of lack of hardware support",
					settings.GetMaxAnisotropy(), maxSupportedAnisotropy);
			}

			float maxAnisotropy = std::max(maxSupportedAnisotropy, settings.GetMaxAnisotropy());
			glSamplerParameterf(m_handle, GL_TEXTURE_MAX_ANISOTROPY, maxAnisotropy);
		}

		glSamplerParameterfv(m_handle, GL_TEXTURE_BORDER_COLOR, &settings.GetBorderColor()[0]);

		glSamplerParameterf(m_handle, GL_TEXTURE_LOD_BIAS, settings.GetLodBias());

		glSamplerParameterf(m_handle, GL_TEXTURE_MIN_LOD, settings.GetMinLod());
		glSamplerParameterf(m_handle, GL_TEXTURE_MAX_LOD, settings.GetMaxLod());
	}

	OpenGLTextureSampler::~OpenGLTextureSampler()
	{
		glDeleteSamplers(1, &m_handle);
	}

}

