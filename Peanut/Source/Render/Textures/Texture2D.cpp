#include <Peanut/Render/Textures/Texture2D.hpp>

#include <Render/Textures/Impl/OpenGLTexture2D.hpp>
#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>

namespace pn {

std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path, const Texture2DSettings& settings, const std::string& name)
{
    auto renderApi = RenderCommand::GetRenderAPI();

    switch (renderApi) {
        case RenderAPI::None:
            PN_CORE_ASSERT(false, "RenderAPI::None is not supported"); 
            break;
        case RenderAPI::OpenGL:
            return std::make_shared<OpenGLTexture2D>(path, settings, name);
        default:
            PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi)); 
            break;
    }

    return nullptr;
}

std::shared_ptr<Texture2D> Texture2D::Create(const TextureData& data, const Texture2DSettings& settings, const std::string& name)
{
    auto renderApi = RenderCommand::GetRenderAPI();

    switch (renderApi) {
        case RenderAPI::None:
            PN_CORE_ASSERT(false, "RenderAPI::None is not supported"); 
            break;
        case RenderAPI::OpenGL:
            return std::make_shared<OpenGLTexture2D>(data, settings, name);
        default:
            PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi)); 
            break;
    }

    return nullptr;
}

}
