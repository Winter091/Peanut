#pragma once

#include <Peanut/Render/Textures/Texture.hpp>

#include <vector>
#include <memory>

namespace pn {

	struct TextureLoadSettings
	{
	public:
		TextureLoadSettings& SetOutFormat(TextureFormat format)
		{
			m_format = format;
			return *this;
		}

		TextureFormat GetOutFormat() const { return m_format; }

	private:
		TextureFormat m_format = TextureFormat::RGBA;
	};

	struct TextureLoadResult
	{
	public:
		TextureLoadResult(std::unique_ptr<uint8_t> data, uint32_t dataSize, uint32_t width, uint32_t height)
			: m_data(std::move(data))
			, m_dataSize(dataSize)
			, m_width(width)
			, m_height(height)
		{
			PN_CORE_ASSERT(m_data, "Pointer to data cannot be nullptr");
			PN_CORE_ASSERT(width && height, "Texture width or/and height cannnot be 0");
		}

		void* GetData() const { return static_cast<void*>(m_data.get()); }
		uint32_t GetDataSize() const { return m_dataSize; }
		uint32_t GetWidth() const { return m_width; }
		uint32_t GetHeight() const { return m_height; }

	private:
		std::unique_ptr<uint8_t> m_data;
		uint32_t m_dataSize;
		uint32_t m_width;
		uint32_t m_height;
	};

	class TextureLoad
	{
	public:
	
		static TextureLoadResult FromFile(const char* filename, const TextureLoadSettings& settings);
	};

}
