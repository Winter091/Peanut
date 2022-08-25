#include <Peanut/Render/Textures/TextureLoad.hpp>

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/TimeProfiler.hpp>
#include <Peanut/Core/Utils/FileUtils.hpp>

#include <stb/stb_image.h>

namespace pn {

    TextureLoadResult TextureLoad::FromFile(const char* filename, const TextureLoadSettings& settings)
	{
        PN_PROFILE_FUNCTION();

        auto fileData = ReadFile(filename);

        int x, y, channels;
        int desiredChannels = static_cast<int>(GetTextureFormatNumChannels(settings.GetOutFormat()));

        stbi_set_flip_vertically_on_load(1);
        auto textureData = stbi_load_from_memory(&fileData[0], static_cast<int>(fileData.size()), &x, &y, &channels, desiredChannels);

        return TextureLoadResult(
            std::unique_ptr<uint8_t>(textureData),
            static_cast<uint32_t>(x * y * desiredChannels),
            static_cast<uint32_t>(x), 
            static_cast<uint32_t>(y));
	}

}
