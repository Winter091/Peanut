#include <Peanut/Render/Shaders/Shader.hpp>

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>

#if defined(PN_RENDERING_OPENGL)
#include <Render/Shaders/Impl/OpenGLShader.hpp>
#endif

#if defined(PN_RENDERING_DX11)
#include <Render/Shaders/Impl/Dx11Shader.hpp>
#endif

namespace pn
{

std::shared_ptr<Shader> Shader::Create(const ShaderPaths& paths, const std::string& name)
{
    auto renderApi = RenderCommand::GetRenderAPI();

    switch (renderApi) {
        case RenderAPI::None:
            PN_CORE_ASSERT(false, "RenderAPI::None is not supported"); 
            break;

#if defined(PN_RENDERING_OPENGL)
        case RenderAPI::OpenGL:
            return std::make_shared<OpenGLShader>(paths, name);
#endif

#if defined(PN_RENDERING_DX11)
        case RenderAPI::Dx11:
        {
            // Nothing is more permanent than something temporary
            auto newPaths = ShaderPaths()
                .SetVertexPath(paths.VertexPath + "_dx11")
                .SetFragmentPath(paths.FragmentPath + "_dx11");
            return std::make_shared<Dx11Shader>(newPaths, name);
        }
#endif

        default:
            PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi)); 
            break;
    }

    return nullptr;
}

std::shared_ptr<Shader> Shader::Create(const ShaderSources& sources, const std::string& name)
{
    auto renderApi = RenderCommand::GetRenderAPI();

    switch (renderApi) {
        case RenderAPI::None:
            PN_CORE_ASSERT(false, "RenderAPI::None is not supported"); 
            break;

#if defined(PN_RENDERING_OPENGL)
        case RenderAPI::OpenGL:
            return std::make_shared<OpenGLShader>(sources, name);
#endif

#if defined(PN_RENDERING_DX11)
        case RenderAPI::Dx11:
            return std::make_shared<Dx11Shader>(sources, name);
#endif

        default:
            PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi)); 
            break;
    }


    return nullptr;
}

}