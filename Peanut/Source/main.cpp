#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <iostream>

#include <Window/Window/Window.hpp>

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    pn::Window::Init();
    auto window = pn::Window::Create(800, 600, "My window");

    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        spdlog::critical("Failed to initialize GLAD!\n");
        return -1;
    }

    {
        auto glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        auto vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        auto renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        
        spdlog::info("Using OpenGL {0}", glVersion);
        spdlog::info("Renderer: {} ({})", vendor, renderer);
    }

    while (true)
    {
        window->Update();
    }

    return 0;
}

