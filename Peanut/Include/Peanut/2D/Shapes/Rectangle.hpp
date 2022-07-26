#pragma once

#include "Shape.hpp"

namespace pn {

class Rectangle : public Shape
{
public:
    Rectangle(const glm::vec2& pos = {0.0f, 0.0f}, const glm::vec2& size = {1.0f, 1.0f}, const glm::vec4& color = glm::vec4(1.0f));
    ~Rectangle() override = default;

    const glm::vec2& GetPosition() const override { return m_position; }
    void SetPosition(const glm::vec2& pos) override;

    float GetRotation() const override { return m_rotation; }
    void SetRotation(float rotation) override;

    const glm::vec2& GetScale() const override { return m_scale; }
    void SetScale(const glm::vec2& scale) override;

    const glm::vec2& GetOrigin() const override { return m_origin; }
    void SetOrigin(const glm::vec2& origin) override;

    const glm::vec2& GetSize() const { return m_size; }
    void SetSize(const glm::vec2& size);

    virtual const glm::vec4& GetColor() const override { return m_color; }
    void SetColor(const glm::vec4& color) override;

    bool HasTexture() const override { return m_texture != nullptr; }
    const std::shared_ptr<Texture2D>& GetTexture() const override { return m_texture; }
    void SetTexture(const std::shared_ptr<Texture2D>& texture) override;

    const glm::mat4& GetTransformMatrix() const override;

private:
    glm::vec2 m_position;
    float m_rotation;
    glm::vec2 m_scale;
    glm::vec2 m_origin;

    glm::vec2 m_size;

    glm::vec4 m_color;
    std::shared_ptr<Texture2D> m_texture;

    mutable glm::mat4 m_transform;
    mutable bool m_transformIsDirty;

private:
    void RecalculateTransform() const;
};

}
