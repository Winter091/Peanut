#include <Peanut/Core/Log.hpp>
#include <Peanut/Application/Application.hpp>
#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/2D/Render/Renderer2D.hpp>

namespace pn::internal {

void InitWithoutContext()
{
    Log::Init();
    Window::Init();
    RenderCommand::Init();
}

void InitWithContext()
{
    Renderer2D::Init();
}

void ShutdownWithContext()
{
    Renderer2D::Shutdown();
}

void ShutdownWithoutContext()
{
    RenderCommand::Shutdown();
    Window::Shutdown();
    Log::Shutdown();
}

}