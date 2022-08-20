#include "OpenGLTextureSampler.hpp"

#include "OpenGLEnumConversions.hpp"

namespace pn {

	OpenGLTextureSampler::OpenGLTextureSampler(const TextureSamplerSettings& settings)
	{
		glGenSamplers(1, &m_handle);

		glSamplerParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GetGlMinFilter(settings.GetMinFilter(), settings.GetMipFilter()));
		glSamplerParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GetGlMagFilter(settings.GetMagFilter()));

		glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_S, GetGLWrap(settings.GetWrapX()));
		glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_T, GetGLWrap(settings.GetWrapY()));

		if (settings.GetMinFilter() != TextureFilter::Anisotropic && settings.GetMagFilter() != TextureFilter::Anisotropic) {
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
	}

	OpenGLTextureSampler::~OpenGLTextureSampler()
	{
		glDeleteSamplers(1, &m_handle);
	}

}

