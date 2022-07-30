#include <Core/Init.hpp>
#include <Core/StoragePath.hpp>

#include <Peanut/Application/CommandLineArgs.hpp>
#include <Peanut/Application/Application.hpp>

int main(int argc, const char** argv)
{
    pn::internal::InitWithoutContext();
    {
        pn::CommandLineArgs args(argc, argv);
        PN_CORE_ENSURE(args.GetSize() >= 2, "Too few launch arguments");
        pn::StoragePath::SetAssetsPath(args[1]);

        pn::Application* app = pn::Application::CreateApplication(args);

        pn::internal::InitWithContext();    
        app->Run();
        pn::internal::ShutdownWithContext();

        delete app;
    }
    pn::internal::ShutdownWithoutContext();

    return 0;
}
