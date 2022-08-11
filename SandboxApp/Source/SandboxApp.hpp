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
    std::shared_ptr<pn::PipelineState> m_pipelineState;
    std::shared_ptr<pn::ConstantBuffer> m_constantBuffer;
    std::shared_ptr<pn::Shader> m_shader;
};
