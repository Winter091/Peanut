#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <iostream>

#include <Window/Window/Window.hpp>

int main()
{
    pn::Window::Init();
    auto window = pn::Window::Create(800, 600, "My window");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        spdlog::critical("Failed to initialize GLAD!\n");
        return -1;
    }
    
    auto glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    auto vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    auto renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    
    spdlog::info("Using OpenGL {0}", glVersion);
    spdlog::info("Renderer: {} ({})", vendor, renderer);

    while (!window->ShouldClose())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        window->Update();
    }

    return 0;
}

