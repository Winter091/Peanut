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
    virtual ~GLFWWindow() override;

    virtual void SetEventCallbackFunc() override;
    virtual void Update() override;
    virtual bool ShouldClose() const override;

    inline GLFWwindow* GetGLFWHandle() { return m_glfwHandle; }

private:
    GLFWwindow* m_glfwHandle;
};

}

#endif