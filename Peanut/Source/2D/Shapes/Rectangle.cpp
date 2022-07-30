#include <Peanut/2D/Shapes/Rectangle.hpp>

#include <Peanut/Core/TimeProfiler.hpp>

#include <glm/gtx/transform.hpp>

namespace pn {

Rectangle::Rectangle(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
    : m_position(pos)
    , m_rotation(0.0f)
    , m_scale({1.0f, 1.0f})
    , m_origin({0.0f, 0.0f})
    , m_size(size)
    , m_color(color)
    , m_transform(1.0f)
    , m_transformIsDirty(true)
{}

void Rectangle::SetPosition(const glm::vec2& pos)
{
    m_position = pos;
    m_transformIsDirty = true;
}

void Rectangle::SetRotation(float rotation)
{
    m_rotation = std::fmod(rotation, glm::two_pi<float>());
    m_transformIsDirty = true;
}

void Rectangle::SetScale(const glm::vec2& scale)
{
    m_scale = scale;
    m_transformIsDirty = true;
}

void Rectangle::SetOrigin(const glm::vec2& origin)
{
    m_origin = origin;
    m_transformIsDirty = true;
}

void Rectangle::SetSize(const glm::vec2& size)
{
    m_size = size;
    m_transformIsDirty = true;
}

void Rectangle::SetColor(const glm::vec4& color)
{
    m_color = color;
}

void Rectangle::SetTexture(const std::shared_ptr<Texture2D>& texture)
{
    m_texture = texture;
}

const glm::mat4& Rectangle::GetTransformMatrix() const 
{
    if (m_transformIsDirty) {
        RecalculateTransform();
        m_transformIsDirty = false;
    }
    return m_transform;
}

void Rectangle::RecalculateTransform() const
{
    m_transform = glm::translate(glm::mat4(1.0f), glm::vec3(m_position, 0.0f));
    m_transform = glm::rotate(m_transform, m_rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    m_transform = glm::scale(m_transform, glm::vec3(m_scale * m_size, 1.0f));
    m_transform = glm::translate(m_transform, glm::vec3(-m_origin, 0.0f));
}

}