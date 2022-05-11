#include <Peanut/Core/Log.hpp>
#include <Peanut/Core/Application.hpp>

#include <memory>

int main(int argc, const char** argv)
{
    pn::Log::Init();
    pn::Application::Init();

    auto app = std::unique_ptr<pn::Application>(pn::Application::CreateApplication());
    app->Run();

    return 0;
}
