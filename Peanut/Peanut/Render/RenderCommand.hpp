#ifndef __RENDERCOMMAND_H__
#define __RENDERCOMMAND_H__

#include <Peanut/Render/RenderAPI.hpp>
#include <glm/vec4.hpp>

#include <memory>

namespace pn {

class RenderCommand 
{
public:
    RenderCommand() = default;
    virtual ~RenderCommand() = default;
    RenderCommand(const RenderCommand&) = delete;
    RenderCommand& operator=(const RenderCommand&) = delete;

    virtual void SetClearColor(const glm::vec4& color) = 0;
    virtual void Clear() = 0;
};

}

#endif