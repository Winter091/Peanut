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
    std::shared_ptr<pn::Texture2D> m_texture;
    std::shared_ptr<pn::OrthoCamera> m_camera;
    pn::TextureData m_textureData;
};
