#ifndef __SANDBOXAPP_H__
#define __SANDBOXAPP_H__

#include <Peanut/Application/Application.hpp>

class SandboxApp : public pn::Application
{
public:
    SandboxApp(const pn::ApplicationOptions& options);
    SandboxApp(const SandboxApp&) = delete;
    SandboxApp& operator=(const SandboxApp&) = delete;
    ~SandboxApp() override = default;

    void OnEvent(const pn::Event& event) override;
    void OnUpdate() override;
};

#endif