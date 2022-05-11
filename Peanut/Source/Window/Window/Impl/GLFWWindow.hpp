#ifndef __GLFWWINDOW_H__
#define __GLFWWINDOW_H__

#include <Window/Window/Window.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace pn {

class GLFWWindow : public Window
{
public:
    GLFWWindow(int width, int height, const std::string& title);
    GLFWWindow(const GLFWWindow&) = delete;
    GLFWWindow& operator=(const GLFWWindow&) = delete;
    ~GLFWWindow() override;

    void MakeContextCurrent() override;
    void SetEventCallbackFunc() override;
    void Update() override;
    bool ShouldClose() const override;
    void* GetNativeHandle() const override;

private:
    GLFWwindow* m_glfwHandle;
};

}

#endif