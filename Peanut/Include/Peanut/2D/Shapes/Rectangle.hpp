#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "Shape.hpp"

namespace pn {

class Rectangle : public Shape
{
public:
    Rectangle(const glm::vec2& pos = {0.0f, 0.0f}, const glm::vec2& size = {1.0f, 1.0f}, const glm::vec4& color = glm::vec4(1.0f));
    ~Rectangle() override = default;

    virtual const glm::vec2& GetPosition() const override { return m_pos; }
    void SetPosition(const glm::vec2& pos) override;

    virtual const glm::vec4& GetColor() const override { return m_color; }
    void SetColor(const glm::vec4& color) override;

    const glm::vec2& GetSize() const { return m_size; }
    void SetSize(const glm::vec2& size);

    bool HasTexture() const override { return m_texture != nullptr; }
    const std::shared_ptr<Texture2D>& GetTexture() const override { return m_texture; }
    void SetTexture(const std::shared_ptr<Texture2D>& texture) override;

    const glm::mat4& GetTransformMatrix() const override { return m_transform; }

private:
    glm::vec2 m_pos;
    glm::vec2 m_size;
    glm::vec4 m_color;
    glm::mat4 m_transform;

    std::shared_ptr<Texture2D> m_texture;

private:
    void RecalculateTransform();
};

}


#endif