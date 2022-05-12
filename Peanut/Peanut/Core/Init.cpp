#include <Peanut/Core/Log.hpp>
#include <Peanut/Application/Application.hpp>
#include <Peanut/Render/Render.hpp>

namespace pn {

void Init()
{
    pn::Log::Init();
    pn::Application::Init();
    pn::Render::Init();
}

}