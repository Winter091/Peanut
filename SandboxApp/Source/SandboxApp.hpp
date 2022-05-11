#ifndef __SANDBOXAPP_H__
#define __SANDBOXAPP_H__

#include <Peanut/Core/Application.hpp>

class SandboxApp : public pn::Application
{
public:
    SandboxApp();
    SandboxApp(const SandboxApp&) = delete;
    SandboxApp& operator=(const SandboxApp&) = delete;
    ~SandboxApp() override = default;

    void OnEvent(const Event& event) override;
    void OnUpdate() override;
};

#endif