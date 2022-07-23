#include <Peanut/Core/Log.hpp>
#include <Peanut/Application/Application.hpp>
#include <Peanut/Render/Commands/RenderCommand.hpp>

namespace pn {

void Init()
{
    Log::Init();
    Window::Init();
    RenderCommand::Init();
}

}