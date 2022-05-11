#include <Peanut/Core/Log.hpp>
#include <Peanut/Application/Application.hpp>

namespace pn {

void Init()
{
    pn::Log::Init();
    pn::Application::Init();
}

}