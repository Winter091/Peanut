#ifndef __SHAPE_H__
#define __SHAPE_H__

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

namespace pn { 

class Shape
{
public:
    virtual ~Shape() = default;

    virtual const glm::vec2& GetPos() const = 0;
    virtual void SetPos(const glm::vec2& pos) = 0;

    virtual const glm::vec4& GetColor() const = 0;
    virtual void SetColor(const glm::vec4& color) = 0;

    virtual const glm::mat4& GetTransformMatrix() const = 0;
};

}

#endif