#pragma once

#include <Peanut/Render/Textures/Texture2D.hpp>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include <memory>

namespace pn { 

class Shape
{
public:
    virtual ~Shape() = default;

    virtual const glm::vec2& GetPosition() const = 0;
    virtual void SetPosition(const glm::vec2& pos) = 0;

    virtual float GetRotation() const = 0;
    virtual void SetRotation(float rotation) = 0;

    virtual const glm::vec2& GetScale() const = 0;
    virtual void SetScale(const glm::vec2& scale) = 0;

    virtual const glm::vec2& GetOrigin() const = 0;
    virtual void SetOrigin(const glm::vec2& origin) = 0;

    virtual const glm::mat4& GetTransformMatrix() const = 0;

    virtual const glm::vec4& GetColor() const = 0;
    virtual void SetColor(const glm::vec4& color) = 0;

    virtual bool HasTexture() const = 0;
    virtual const std::shared_ptr<Texture2D>& GetTexture() const = 0;
    virtual void SetTexture(const std::shared_ptr<Texture2D>& texture) = 0;
};

}
