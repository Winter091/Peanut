#include <Peanut/Core/Log.hpp>
#include <Peanut/Application/Application.hpp>
#include <Peanut/Core/CommandLineArgs.hpp>

#include <memory>

int main(int argc, const char** argv)
{
    pn::Log::Init();
    pn::Application::Init();

    pn::CommandLineArgs args(argc, argv);

    auto app = std::unique_ptr<pn::Application>(pn::Application::CreateApplication(args));
    app->Run();

    return 0;
}
