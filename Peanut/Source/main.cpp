#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Core/Log.hpp>

#include <iostream>

#include <Window/Window/Window.hpp>

int main()
{
    pn::Log::Init();
    pn::Window::Init();

    auto window = pn::Window::Create(800, 600, "My window");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        PN_CORE_CRITICAL("Failed to initialize GLAD!\n");
        return -1;
    }

    auto glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    auto vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    auto renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    
    PN_CORE_INFO("==========================================");
    PN_CORE_INFO("\tUsing OpenGL {}", glVersion);
    PN_CORE_INFO("\tVendor: {}", vendor);
    PN_CORE_INFO("\tRenderer: {}", renderer);
    PN_CORE_INFO("==========================================");

    while (!window->ShouldClose())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        window->Update();
    }

    return 0;
}

