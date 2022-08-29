#pragma once

#include <Peanut/Window/Window.hpp>
#include <Peanut/Window/WindowSettings.hpp>
#include <Peanut/Events/EventQueue.hpp>
#include <Peanut/Events/WindowEvents.hpp>
#include "CommandLineArgs.hpp"
#include <memory>

namespace pn {

    class Application
    {
    public:
        Application(const WindowSettings& settings);
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;
        virtual ~Application();

        Window& GetWindow() { return *m_window; }

        virtual void OnEvent(Event& event) = 0;
        virtual void OnUpdate() = 0;

        void Run();

        static Application* CreateApplication(const CommandLineArgs& args);
        static Application& GetInstance();

    private:
        std::unique_ptr<Window> m_window;
        EventQueue m_eventQueue;

    private:
        void MainOnEvent(Event& event);
        void MainOnUpdate();
        void SendEvents();
        bool OnWindowResize(WindowSizeChangedEvent& resizeEvent);
    };

}
