#ifndef __GLFWWINDOW_H__
#define __GLFWWINDOW_H__

#include <Window/Window/Window.hpp>

namespace pn {

class GLFWWindow : public Window
{
public:
    GLFWWindow();
    GLFWWindow(const GLFWWindow&) = delete;
    GLFWWindow& operator=(const GLFWWindow&) = delete;
    virtual ~GLFWWindow() override;

    virtual void SetEventCallbackFunc() override;
    virtual void Update() override;

private:
};

}

#endif