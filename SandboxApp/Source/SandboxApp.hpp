#ifndef __SANDBOXAPP_H__
#define __SANDBOXAPP_H__

#include <Peanut/Application/Application.hpp>

#include <Peanut/Render/Buffers/VertexBuffer.hpp>
#include <Peanut/Render/Buffers/VertexArray.hpp>
#include <Peanut/Render/Shaders/Shader.hpp>

class SandboxApp final : public pn::Application
{
public:
    SandboxApp(const pn::WindowSettings& settings);
    SandboxApp(const SandboxApp&) = delete;
    SandboxApp& operator=(const SandboxApp&) = delete;
    ~SandboxApp() override = default;

    void OnEvent(const pn::Event& event) override;
    void OnUpdate() override;

private:
    std::shared_ptr<pn::VertexArray> m_triangleVAO;
    std::shared_ptr<pn::Shader> m_shader;
};

#endif