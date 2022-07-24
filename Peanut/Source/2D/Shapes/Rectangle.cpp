#include <Peanut/2D/Shapes/Rectangle.hpp>

#include <glm/gtx/transform.hpp>

namespace pn {

Rectangle::Rectangle(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
    : Shape(pos, color)
    , m_size(size)
{
    RecalculateTransform();
}

void Rectangle::SetPos(const glm::vec2& pos)
{
    m_pos = pos;
    RecalculateTransform();
}

void Rectangle::SetColor(const glm::vec4& color)
{
    m_color = color;
}

void Rectangle::SetSize(const glm::vec2& size)
{
    m_size = size;
    RecalculateTransform();   
}

void Rectangle::RecalculateTransform()
{
    m_transform = glm::translate(glm::vec3(m_pos + m_size / 2.0f, 0.0f)) *
                  glm::scale(glm::vec3(m_size, 1.0f));
}


const glm::mat4& Rectangle::GetTransformMatrix() const
{
    return m_transform;
}

}