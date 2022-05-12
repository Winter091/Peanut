#include <Peanut/Core/Log.hpp>
#include <Peanut/Application/Application.hpp>
#include <Peanut/Render/RenderCommand.hpp>

namespace pn {

void Init()
{
    pn::Log::Init();
    pn::Application::Init();
    pn::RenderCommand::Init();
}

}