#include <Core/Init.hpp>
#include <Core/StoragePath.hpp>

#include <Peanut/Application/CommandLineArgs.hpp>
#include <Peanut/Application/Application.hpp>
#include <Peanut/Core/TimeProfiler.hpp>

int main(int argc, const char** argv)
{
    pn::Application* app = nullptr;
    
    PN_PROFILE_BEGIN_SESSION("Init", "init_profiling_data.json");
    {
        pn::internal::InitWithoutContext();
        
        pn::CommandLineArgs args(argc, argv);
        PN_CORE_ENSURE(args.GetSize() >= 2, "Too few launch arguments");
        pn::StoragePath::SetAssetsPath(args[1]);

        app = pn::Application::CreateApplication(args);
        
        pn::internal::InitWithContext();
    }
    PN_PROFILE_END_SESSION();

    PN_PROFILE_BEGIN_SESSION("Run", "run_profiling_data.json");
    {
        app->Run();
    }
    PN_PROFILE_END_SESSION();

    PN_PROFILE_BEGIN_SESSION("Shutdown", "shutdown_profiling_data.json");
    {
        pn::internal::ShutdownWithContext();
        delete app;
        pn::internal::ShutdownWithoutContext();
    }
    PN_PROFILE_END_SESSION();

    return 0;
}
