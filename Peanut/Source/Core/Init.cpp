#include <Peanut/Core/Log.hpp>
#include <Peanut/Application/Application.hpp>
#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/2D/Render/Renderer2D.hpp>
#include <Peanut/Core/TimeProfiler.hpp>

namespace pn::internal {

void InitWithoutContext()
{
    PN_PROFILE_FUNCTION();

    Log::Init();
    Window::Init();
    RenderCommand::Init();
}

void InitWithContext()
{
    PN_PROFILE_FUNCTION();

    Renderer2D::Init();
}

void ShutdownWithContext()
{
    PN_PROFILE_FUNCTION();

    Renderer2D::Shutdown();
}

void ShutdownWithoutContext()
{
    PN_PROFILE_FUNCTION();

    RenderCommand::Shutdown();
    Window::Shutdown();
    Log::Shutdown();
}

}