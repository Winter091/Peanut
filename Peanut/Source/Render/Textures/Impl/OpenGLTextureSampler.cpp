#include "OpenGLTextureSampler.hpp"

#include "OpenGLEnumConversions.hpp"

#include <glad/glad.h>

namespace pn {

	OpenGLTextureSampler::OpenGLTextureSampler(const TextureSamplerSettings& settings)
	{
		glGenSamplers(1, &m_handle);

		glSamplerParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GetGlMinFilter(settings.GetMinFilter(), settings.GetMipFilter()));
		glSamplerParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GetGlMagFilter(settings.GetMagFilter()));

		glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_S, GetGLWrap(settings.GetWrapX()));
		glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_T, GetGLWrap(settings.GetWrapY()));
	}

	OpenGLTextureSampler::~OpenGLTextureSampler()
	{
		glDeleteSamplers(1, &m_handle);
	}

}

