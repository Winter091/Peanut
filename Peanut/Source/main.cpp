#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Core/Log.hpp>

#include <Core/Application.hpp>
#include <SandboxApp.hpp>

int main()
{
    pn::Log::Init();
    pn::Application::Init();

    SandboxApp app;
    app.Run();

    return 0;
}

