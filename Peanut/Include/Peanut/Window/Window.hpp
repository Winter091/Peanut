#pragma once

#include <Peanut/Window/RenderContext.hpp>
#include <Peanut/Window/WindowProvider.hpp>
#include <Peanut/Events/Event.hpp>
#include <Peanut/Window/WindowSettings.hpp>
#include <Peanut/Core/KeyCodes.hpp>
#include <Peanut/Core/MouseCodes.hpp>
#include <glm/vec2.hpp>
#include <memory>
#include <string>
#include <functional>

namespace pn {

    struct WindowSizeSettings
    {
    public:
        uint32_t Width;
        uint32_t Height;
        bool IsFullScreen;

    public:
        WindowSizeSettings& SetWidth(uint32_t width) { Width = width; return *this; }
        WindowSizeSettings& SetHeight(uint32_t height) { Height = height; return *this; }
        WindowSizeSettings& SetIsFullScreen(bool isFullScreen) { IsFullScreen = isFullScreen; return *this; }
    };

    class Window 
    {
    public:
        using EventCallbackFunc = std::function<void(Event&)>;

        Window() = default;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        virtual ~Window() = default;

        static void Init();
        static void Shutdown();
        static std::unique_ptr<Window> Create(const WindowSettings& settings);
        static WindowProvider GetWindowProvider();
        
        virtual RenderContext& GetRenderContext() = 0;
        virtual void MakeContextCurrent() = 0;
        virtual void SetEventCallbackFunc(const EventCallbackFunc& func) = 0;
        virtual void Update() = 0;
        virtual bool ShouldClose() const = 0;

        virtual void* GetNativeHandle() const = 0;
        virtual int GetWidth() const = 0;
        virtual int GetHeight() const = 0;
        virtual float GetAspectRatio() const = 0;
        virtual bool GetIsFullScreen() const = 0;

        virtual void SetSize(const WindowSizeSettings& settings) = 0;
    
        virtual const std::string& GetTitle() const = 0;
        virtual void SetTitle(const std::string& title) = 0;

        virtual void SetSwapInterval(int interval) = 0;

        virtual bool IsKeyPressed(KeyCode key) const = 0;
        virtual bool IsMouseButtonPressed(MouseButtonCode button) const = 0;
        virtual glm::vec2 GetCursorPosition() const = 0;

    private:
        static WindowProvider s_windowProvider;
        static bool s_isInitialized;
    };

}
