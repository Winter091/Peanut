#include "TextureLoad.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/TimeProfiler.hpp>
#include <Peanut/Core/Utils/FileUtils.hpp>

#include <stb/stb_image.h>

namespace pn {

    static uint32_t GetNumChannels(TextureFormat format)
    {
        switch (format) {
            case TextureFormat::RGB:    return 3u;
            case TextureFormat::RGBA:   return 4u;
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown TextureFormat enum value");
        return 0u;
    }


    TextureLoadResult TextureLoad::FromFile(const char* filename, const TextureLoadSettings& settings)
	{
        PN_PROFILE_FUNCTION();

        auto fileData = ReadFile(filename);

        int x, y, channels;
        int desiredChannels = static_cast<int>(GetNumChannels(settings.GetOutFormat()));

        stbi_set_flip_vertically_on_load(1);
        auto textureData = stbi_load_from_memory(&fileData[0], static_cast<int>(fileData.size()), &x, &y, &channels, desiredChannels);

        return TextureLoadResult(
            std::unique_ptr<uint8_t>(textureData),
            static_cast<uint32_t>(x * y * desiredChannels),
            static_cast<uint32_t>(x), 
            static_cast<uint32_t>(y));
	}

}
