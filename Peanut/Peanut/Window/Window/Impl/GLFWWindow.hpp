#ifndef __GLFWWINDOW_H__
#define __GLFWWINDOW_H__

#include <Peanut/Window/Window/Window.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace pn {

class GLFWWindow final : public Window
{
public:
    GLFWWindow(const WindowSettings& settings);
    GLFWWindow(const GLFWWindow&) = delete;
    GLFWWindow& operator=(const GLFWWindow&) = delete;
    ~GLFWWindow() override;

    void MakeContextCurrent() override;
    void SetEventCallbackFunc(const EventCallbackFunc& func) override;
    void Update() override;
    bool ShouldClose() const override;

    void* GetNativeHandle() const override;
    int GetWidth() const override { return m_data.width; }
    int GetHeight() const override { return m_data.height; }

    const std::string& GetTitle() const override { return m_data.title; }
    void SetTitle(const std::string& title) override;

    void SetSwapInterval(int interval) override;

private:
    static int s_windowCount;

    // m_data will be available using glfwGetWindowUserPointer(),
    // it's required while setting up callbacks
    struct WindowData {
        GLFWWindow* thisPtr;
        GLFWwindow* glfwHandle;

        int width;
        int height;
        int swapInterval;
        std::string title;

        EventCallbackFunc eventCallbackFunc;
    } m_data;

private:
    void SetupGlfwCallbacks();
    static void OnWindowCreate();
    static void OnWindowDestroy();
};

}

#endif