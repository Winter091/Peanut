#include "RenderCommand.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/PlatformDetection.hpp>
#include <Peanut/Render/Impl/OpenGLRenderCommand.hpp>

namespace pn
{


// Static members
RenderAPI RenderCommand::s_renderAPI;
bool RenderCommand::s_isInitialized = false;
std::unique_ptr<RenderCommandImpl> RenderCommand::s_renderCommandImpl;


void RenderCommand::Init()
{
    PN_CORE_ASSERT(!s_isInitialized, "RenderCommand can be initialized only once");
    s_isInitialized = true;

#if defined(PN_PLATFORM_WINDOWS)
    s_renderAPI = RenderAPI::OpenGL;
    s_renderCommandImpl = std::make_unique<OpenGLRenderCommand>();
#elif defined(PN_PLATFORM_LINUX)
    s_renderAPI = RenderAPI::OpenGL;
    s_renderCommandImpl = std::make_unique<OpenGLRenderCommand>();
#else
    PN_CORE_ASSERT("RenderCommand: platform is not supported");
#endif
}

RenderAPI RenderCommand::GetRenderAPI()
{
    PN_CORE_ASSERT(s_isInitialized, "RenderCommand has to be initialized before usage");
    return s_renderAPI;
}

void RenderCommand::SetClearColor(const glm::vec4& color)
{
    PN_CORE_ASSERT(s_isInitialized, "RenderCommand has to be initialized before usage");
    return s_renderCommandImpl->SetClearColor(color);
}

void RenderCommand::Clear()
{
    PN_CORE_ASSERT(s_isInitialized, "RenderCommand has to be initialized before usage");
    return s_renderCommandImpl->Clear();
}

void RenderCommand::DrawArrays(std::shared_ptr<VertexArray>& vertexArray, uint32_t count)
{
    PN_CORE_ASSERT(s_isInitialized, "RenderCommand has to be initialized before usage");
    s_renderCommandImpl->DrawArrays(vertexArray, count);
}

}