#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "Shape.hpp"

namespace pn {

class Rectangle : public Shape
{
public:
    Rectangle(const glm::vec2& pos = {0.0f, 0.0f}, const glm::vec2& size = {1.0f, 1.0f}, const glm::vec4& color = glm::vec4(1.0f));
    ~Rectangle() override = default;

    void SetPos(const glm::vec2& pos) override;
    void SetColor(const glm::vec4& color) override;

    const glm::vec2& GetSize() const { return m_size; }
    void SetSize(const glm::vec2& size);

    const glm::mat4& GetTransformMatrix() const override;

private:
    glm::vec2 m_size;

private:
    void RecalculateTransform();
};

}


#endif