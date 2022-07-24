#ifndef __SHAPE_H__
#define __SHAPE_H__

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

namespace pn { 

class Shape
{
public:
    Shape(const glm::vec2& pos = { 0.0f, 0.0f }, const glm::vec4& color = glm::vec4(1.0f));
    virtual ~Shape() = default;

    virtual const glm::vec2& GetPos() const { return m_pos; }
    virtual void SetPos(const glm::vec2& pos) = 0;

    virtual const glm::vec4& GetColor() const { return m_color; }
    virtual void SetColor(const glm::vec4& color) = 0;

    virtual const glm::mat4& GetTransformMatrix() const = 0;

protected:
    glm::vec2 m_pos;
    glm::vec4 m_color;
    glm::mat4 m_transform;
};

}

#endif