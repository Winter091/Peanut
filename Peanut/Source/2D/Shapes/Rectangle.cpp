#include <Peanut/2D/Shapes/Rectangle.hpp>

#include <glm/gtx/transform.hpp>

namespace pn {

Rectangle::Rectangle(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
    : m_pos(pos)
    , m_size(size)
    , m_color(color)
{
    RecalculateTransform();
}

void Rectangle::SetPosition(const glm::vec2& pos)
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

void Rectangle::SetTexture(const std::shared_ptr<Texture2D>& texture)
{
    m_texture = texture;
}

void Rectangle::RecalculateTransform()
{
    m_transform = glm::translate(glm::vec3(m_pos + m_size / 2.0f, 0.0f)) *
                  glm::scale(glm::vec3(m_size, 1.0f));
}

}