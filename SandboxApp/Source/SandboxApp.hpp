#pragma once

#include <Peanut/Peanut.hpp>

class SandboxApp final : public pn::Application
{
public:
    SandboxApp(const pn::WindowSettings& settings);
    SandboxApp(const SandboxApp&) = delete;
    SandboxApp& operator=(const SandboxApp&) = delete;
    ~SandboxApp() override = default;

    void OnEvent(pn::Event& event) override;
    void OnUpdate() override;

private:
    std::vector<pn::Rectangle> m_rectangles;
    std::shared_ptr<pn::OrthoCamera> m_camera;

    pn::Rectangle m_texRectangle;
    std::shared_ptr<pn::OrthoCamera> m_texCamera;

    std::shared_ptr<pn::Framebuffer> m_framebuffer;
};
