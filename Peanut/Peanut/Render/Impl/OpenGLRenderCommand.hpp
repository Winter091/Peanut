#ifndef __OPENGLRENDERCOMMAND_H__
#define __OPENGLRENDERCOMMAND_H__

#include <Peanut/Render/RenderCommandImpl.hpp>

namespace pn {

class OpenGLRenderCommand : public RenderCommandImpl
{
public:
    OpenGLRenderCommand();
    ~OpenGLRenderCommand() override = default;

    void SetClearColor(const glm::vec4& color) override;
    void Clear() override;
};

}

#endif