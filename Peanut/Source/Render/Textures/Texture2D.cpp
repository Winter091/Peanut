#include <Peanut/Render/Textures/Texture2D.hpp>

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>

#if defined(PN_RENDERING_OPENGL)
#include <Render/Textures/Impl/OpenGLTexture2D.hpp>
#endif

#if defined(PN_RENDERING_DX11)
#include <Render/Textures/Impl/Dx11Texture2D.hpp>
#endif

namespace pn {

glm::u32vec2 Texture2D::GetMipLevelDimensions(const glm::u32vec2& mainDimensions, uint32_t level)
{
    glm::u32vec2 result = mainDimensions;

    for (uint32_t i = 0; i < level; i++) {
        result.x = std::max<uint32_t>(1u, result.x / 2);
        result.y = std::max<uint32_t>(1u, result.y / 2);

        if (result.x == 1 && result.y == 1) {
            break;
        }
    }

    return result;
}

uint32_t Texture2D::GetMaxAmountOfMips(const glm::u32vec2& mainDimensions)
{
    return 1u + static_cast<uint32_t>(std::floor(std::log2f(static_cast<float>(std::max(mainDimensions.x, mainDimensions.y)))));
}

std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path, const Texture2DSettings& settings, const std::string& name)
{
    auto renderApi = RenderCommand::GetRenderAPI();

    switch (renderApi) {
        case RenderAPI::None:
            PN_CORE_ASSERT(false, "RenderAPI::None is not supported"); 
            break;

#if defined(PN_RENDERING_OPENGL)
        case RenderAPI::OpenGL:
            return std::make_shared<OpenGLTexture2D>(path, settings, name);
#endif

#if defined(PN_RENDERING_DX11)
        case RenderAPI::Dx11:
            return std::make_shared<Dx11Texture2D>(path, settings, name);
#endif

        default:
            PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi)); 
            break;
    }

    return nullptr;
}

std::shared_ptr<Texture2D> Texture2D::Create(const void* data, const Texture2DSettings& settings, const std::string& name)
{
    auto renderApi = RenderCommand::GetRenderAPI();

    switch (renderApi) {
        case RenderAPI::None:
            PN_CORE_ASSERT(false, "RenderAPI::None is not supported"); 
            break;

#if defined(PN_RENDERING_OPENGL)
        case RenderAPI::OpenGL:
            return std::make_shared<OpenGLTexture2D>(data, settings, name);
#endif

#if defined(PN_RENDERING_DX11)
        case RenderAPI::Dx11:
            return std::make_shared<Dx11Texture2D>(data, settings, name);
#endif

        default:
            PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi)); 
            break;
    }

    return nullptr;
}

}
