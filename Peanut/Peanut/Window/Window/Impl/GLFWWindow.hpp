#ifndef __GLFWWINDOW_H__
#define __GLFWWINDOW_H__

#include <Peanut/Window/Window/Window.hpp>
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
    void SetEventCallbackFunc(const EventCallbackFunc& func) override;
    void Update() override;
    bool ShouldClose() const override;
    void* GetNativeHandle() const override;

    int GetWidth() const override { return m_data.m_width; }
    int GetHeight() const override { return m_data.m_height; }
    const std::string& GetTitle() const override { return m_data.m_title; }

private:
    static int s_windowCount;

    // m_data will be available using glfwGetWindowUserPointer(),
    // it's required while setting up callbacks
    struct WindowData {
        GLFWwindow* m_glfwHandle;

        int m_width;
        int m_height;
        std::string m_title;

        EventCallbackFunc eventCallbackFunc;
    } m_data;

private:
    void SetupGlfwCallbacks();
    static void OnWindowCreate();
    static void OnWindowDestroy();
};

}

#endif