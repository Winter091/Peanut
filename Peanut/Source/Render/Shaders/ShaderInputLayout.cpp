#include <Peanut/Render/Shaders/ShaderInputLayout.hpp>

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>

#include <unordered_set>

#if defined(PN_RENDERING_OPENGL)
#include <Render/Shaders/Impl/OpenGLShaderInputLayout.hpp>
#endif

#if defined(PN_RENDERING_DX11)
#include <Render/Shaders/Impl/Dx11ShaderInputLayout.hpp>
#endif

namespace pn {

    std::shared_ptr<ShaderInputLayout> ShaderInputLayout::Create(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers, const std::shared_ptr<Shader>& shader)
    {    
        auto renderApi = RenderCommand::GetRenderAPI();

        std::shared_ptr<ShaderInputLayout> result = nullptr;

        switch (renderApi) {
            case RenderAPI::None:
                PN_CORE_ASSERT(false, "RenderAPI::None is not supported");
                break;

#if defined(PN_RENDERING_OPENGL)
            case RenderAPI::OpenGL:
                (void)vertexBuffers;
                (void)shader;
                result = std::make_shared<OpenGLShaderInputLayout>();
                break;
#endif

#if defined(PN_RENDERING_DX11)
            case RenderAPI::Dx11:
                result = std::make_shared<Dx11ShaderInputLayout>(vertexBuffers, shader);
                break;
#endif

            default:
                PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi));
                break;
        }

        PN_CORE_ASSERT(CheckIfLayoutIsCorrect(vertexBuffers), "Shader input layout is incorrect");

        return result;
    }

    bool ShaderInputLayout::CheckIfLayoutIsCorrect(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers)
    {
        std::unordered_set<uint32_t> seenIndices;

        for (const auto& buffer : vertexBuffers) {
            const auto& bufferAttributes = buffer->GetLayout()->GetElements();

            for (const BufferLayoutElement& elem : bufferAttributes) {
                if (!seenIndices.insert(elem.GetIndex()).second) {
                    PN_CORE_ASSERT(false, "At least 2 attributes in shader input layout have the same index = {}", elem.GetIndex());
                    return false;
                }

                if (elem.GetType() == BufferLayoutElementType::Mat4) {
                    for (int offset = 1; offset < 4; offset++) {
                        if (!seenIndices.insert(elem.GetIndex() + offset).second) {
                            PN_CORE_ASSERT(false, "Mat4 in shader input layout takes 4 consequtive indices, but they're occupied");
                        }
                    }
                }
            }
        }

        return true;
    }

}
