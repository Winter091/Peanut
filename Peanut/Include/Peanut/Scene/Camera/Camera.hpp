#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace pn {

class Camera
{
public:
    virtual ~Camera() = default;

    virtual void SetPosition(const glm::vec3& pos) = 0;
    virtual const glm::vec3& GetPosition() const = 0;

    virtual void SetDirection(const glm::vec3& direction) = 0;
    virtual void LookAt(const glm::vec3& point) = 0;
    virtual const glm::vec3& GetDirection() const = 0;

    virtual void SetUp(const glm::vec3& up) = 0;
    virtual const glm::vec3& GetUp() const = 0;

    virtual void SetAspectRatio(float aspectRatio) = 0;
    virtual float GetAspectRatio() const = 0;

    virtual const glm::mat4& GetViewMatrix() const = 0;
    virtual const glm::mat4& GetProjectionMatrix() const = 0;
    virtual const glm::mat4& GetViewProjectionMatrix() const = 0;
};

}

