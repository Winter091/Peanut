#include <Peanut/Core/Init.hpp>
#include <Peanut/Core/CommandLineArgs.hpp>
#include <Peanut/Application/Application.hpp>

int main(int argc, const char** argv)
{
    pn::Init();

    pn::CommandLineArgs args(argc, argv);

    auto app = std::unique_ptr<pn::Application>(pn::Application::CreateApplication(args));
    app->Run();

    return 0;
}
