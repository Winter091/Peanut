#pragma once

#include <Peanut/Window/Window.hpp>
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

        RenderContext& GetRenderContext() override { return *m_data.renderContext; }
        void MakeContextCurrent() override;
        void SetEventCallbackFunc(const EventCallbackFunc& func) override;
        void Update() override;
        bool ShouldClose() const override;

        void* GetNativeHandle() const override;
        int GetWidth() const override { return m_data.width; }
        int GetHeight() const override { return m_data.height; }
        float GetAspectRatio() const override { return static_cast<float>(m_data.width) / static_cast<float>(m_data.height); }
        bool GetIsFullScreen() const override { return m_data.isFullScreen; }

        void SetSize(const WindowSizeSettings& settings) override;

        const std::string& GetTitle() const override { return m_data.title; }
        void SetTitle(const std::string& title) override;

        void SetSwapInterval(int interval) override;

        bool IsKeyPressed(KeyCode key) const override;
        bool IsMouseButtonPressed(MouseButtonCode button) const override;
        glm::vec2 GetCursorPosition() const override;

    private:
        static int s_windowCount;

        // m_data will be available using glfwGetWindowUserPointer(),
        // it's required while setting up callbacks
        struct WindowData {
            Window* thisPtr;
            std::unique_ptr<RenderContext> renderContext; 

            GLFWwindow* glfwHandle;

            int width;
            int height;
            int swapInterval;
            std::string title;
            bool isFullScreen;

            int prevWindowedXPos = 100;
            int prevWindowedYPos = 100;

            EventCallbackFunc eventCallbackFunc;
        } m_data;

    private:
        void SetupGlfwCallbacks();
        static void OnWindowCreate();
        static void OnWindowDestroy();
        static int KeyCodeToGlfwInt(KeyCode key);
        static int MouseButtonCodeToGlfwInt(MouseButtonCode button);
    };

}
