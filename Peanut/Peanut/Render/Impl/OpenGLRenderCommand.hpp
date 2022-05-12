#ifndef __OPENGLRENDERCOMMAND_H__
#define __OPENGLRENDERCOMMAND_H__

#include <Peanut/Render/RenderCommand.hpp>

namespace pn {

class OpenGLRenderCommand : public RenderCommand
{
public:
    OpenGLRenderCommand();
    ~OpenGLRenderCommand() override = default;

    void ClearColor() override;
    void ClearDepth() override;
};

}

#endif