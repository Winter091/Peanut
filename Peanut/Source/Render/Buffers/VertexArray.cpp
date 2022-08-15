#include <Peanut/Render/Buffers/VertexArray.hpp>

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Core/Assert.hpp>

#if defined(PN_RENDERING_OPENGL)
#include <Render/Buffers/Impl/OpenGLVertexArray.hpp>
#endif

#if defined(PN_RENDERING_DX11)
#include <Render/Buffers/Impl/Dx11VertexArray.hpp>
#endif

namespace pn
{

std::shared_ptr<VertexArray> VertexArray::Create(const VertexArrayDescription& description)
{
    auto renderApi = RenderCommand::GetRenderAPI();

    switch (renderApi) {
        case RenderAPI::None:
            PN_CORE_ASSERT(false, "RenderAPI::None is not supported"); 
            break;

#if defined(PN_RENDERING_OPENGL)
        case RenderAPI::OpenGL:
            return std::make_shared<OpenGLVertexArray>(description);
#endif

#if defined(PN_RENDERING_DX11)
        case RenderAPI::Dx11:
            return std::make_shared<Dx11VertexArray>(description);
#endif

        default:
            PN_CORE_ASSERT(false, "Unknown RednerAPI: {}", static_cast<uint32_t>(renderApi)); 
            break;
    }


    return nullptr; 
}

}