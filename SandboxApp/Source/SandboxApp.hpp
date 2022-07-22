#ifndef __SANDBOXAPP_H__
#define __SANDBOXAPP_H__

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
    std::shared_ptr<pn::VertexArray> m_rectangleVAO;
    std::shared_ptr<pn::Shader> m_shader;
    std::shared_ptr<pn::Texture2D> m_texture;
};

#endif