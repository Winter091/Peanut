#include <Peanut/Render/Shaders/ShaderInputLayout.hpp>

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>

#include <Render/Shaders/Impl/OpenGLShaderInputLayout.hpp>
#include <Render/Shaders/Impl/Dx11ShaderInputLayout.hpp>

namespace pn {

std::shared_ptr<ShaderInputLayout> ShaderInputLayout::Create(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers, const std::shared_ptr<Shader>& shader)
{
    auto renderApi = RenderCommand::GetRenderAPI();

    switch (renderApi) {
    case RenderAPI::None:
        PN_CORE_ASSERT(false, "RenderAPI::None is not supported");
        break;
    case RenderAPI::OpenGL:
        return std::make_shared<OpenGLShaderInputLayout>();
    case RenderAPI::Dx11:
        return std::make_shared<Dx11ShaderInputLayout>(vertexBuffers, shader);
    default:
        PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi));
        break;
    }

    return nullptr;
}

}


