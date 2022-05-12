#include "Render.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/PlatformDetection.hpp>
#include <Peanut/Render/Impl/OpenGLRenderCommand.hpp>

namespace pn
{


// Static members
RenderAPI Render::s_renderAPI;
bool Render::s_isInitialized = false;
std::unique_ptr<RenderCommand> Render::s_renderCommand;


void Render::Init()
{
    PN_CORE_ASSERT(!s_isInitialized, "RenderCommand can be initialized only once");
    s_isInitialized = true;

#if defined(PN_PLATFORM_WINDOWS)
    s_renderAPI = RenderAPI::OpenGL;
    s_instance = std::make_unique<OpenGLRenderCommand>();
#elif defined(PN_PLATFORM_LINUX)
    s_renderAPI = RenderAPI::OpenGL;
    s_renderCommand = std::make_unique<OpenGLRenderCommand>();
#else
    PN_CORE_ASSERT("RenderCommand: platform is not supported");
#endif
}

RenderAPI Render::GetRenderAPI()
{
    PN_CORE_ASSERT(s_isInitialized, "RenderCommand has to be initialized before usage");
    return s_renderAPI;
}

void Render::ClearColor()
{
    PN_CORE_ASSERT(s_isInitialized, "RenderCommand has to be initialized before usage");
    return s_renderCommand->ClearColor();
}

void Render::ClearDepth()
{
    PN_CORE_ASSERT(s_isInitialized, "RenderCommand has to be initialized before usage");
    return s_renderCommand->ClearDepth();
}

}