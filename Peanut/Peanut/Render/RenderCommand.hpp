#ifndef __RENDERCOMMAND_H__
#define __RENDERCOMMAND_H__

#include <Peanut/Render/RenderAPI.hpp>

#include <memory>

namespace pn {

class RenderCommand 
{
public:
    RenderCommand() = default;
    virtual ~RenderCommand() = default;
    RenderCommand(const RenderCommand&) = delete;
    RenderCommand& operator=(const RenderCommand&) = delete;

    virtual void ClearColor() = 0;
    virtual void ClearDepth() = 0;
};

}

#endif