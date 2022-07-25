#ifndef __ORTHOCAMERA_H__
#define __ORTHOCAMERA_H__

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Scene/Camera/Camera.hpp>

namespace pn {

struct OrthoCameraBoundaries
{
public:
    float Left = -1.0f;
    float Right = 1.0f;
    float Bottom = -1.0f;
    float Top = 1.0f;
    float Near = 0.01f;
    float Far = 1000.0f;

private:
    glm::vec2 m_center = { 0.0f, 0.0f };
    bool m_centerIsSet = false;

public:
    OrthoCameraBoundaries& SetCenter(const glm::vec2& center)
    {
        m_center = center;
        m_centerIsSet = true;
        return *this;
    }

    OrthoCameraBoundaries& SetWidth(float width)
    {
        PN_CORE_ASSERT(m_centerIsSet, "Unable to set width, set center first");
        Left = m_center.x - width / 2.0f;
        Right = m_center.x + width / 2.0f;
        return *this;
    }

    OrthoCameraBoundaries& SetHeight(float height)
    {
        PN_CORE_ASSERT(m_centerIsSet, "Unable to set height, set center first");
        Bottom = m_center.y - height / 2.0f;
        Top = m_center.y + height / 2.0f;
        return *this;
    }

    OrthoCameraBoundaries& SetLeft(float left)     { Left = left; return *this; }
    OrthoCameraBoundaries& SetRight(float right)   { Right = right; return *this; }
    OrthoCameraBoundaries& SetBottom(float bottom) { Bottom = bottom; return *this; }
    OrthoCameraBoundaries& SetTop(float top)       { Top = top; return *this; }
    OrthoCameraBoundaries& SetNear(float near)     { Near = near; return *this; }
    OrthoCameraBoundaries& SetFar(float far)       { Far = far; return *this; }
};

struct OrthoCameraSettings
{
public:
    OrthoCameraBoundaries Boundaries;
    glm::vec3 Position = { 0.0f, 0.0f, 1.0f };
    glm::vec3 Direction = { 0.0f, 0.0f, -1.0f };
    glm::vec3 Up = { 0.0f, 1.0f, 0.0f };
    float AspectRatio = 1.0f;
    float Zoom = 1.0f;

private:
    glm::vec3 m_lookAt = { 0.0f, 0.0f, 0.0f };
    bool m_lookAtIsSet = false;

public:
    OrthoCameraSettings& SetBoundaries(const OrthoCameraBoundaries& boundaries)
    {
        Boundaries = boundaries;
        return *this;
    }

    OrthoCameraSettings& SetPosition(const glm::vec3& pos)
    {
        Position = pos;
        if (m_lookAtIsSet) {
            Direction = m_lookAt - Position;
        }
        return *this;
    }

    OrthoCameraSettings& SetDirection(const glm::vec3& dir)
    {
        PN_CORE_ASSERT(!m_lookAtIsSet, "Unable to set direction because LookAt was already specified");
        Direction = dir;
        return *this;
    }

    OrthoCameraSettings& LookAt(const glm::vec3& point)
    {
        Direction = point - Position;
        m_lookAt = point;
        m_lookAtIsSet = true;
        return *this;
    }

    OrthoCameraSettings& SetUp(const glm::vec3& up)         { Up = up; return *this; }
    OrthoCameraSettings& SetAspectRatio(float aspectRatio)  { AspectRatio = aspectRatio; return *this; }
    OrthoCameraSettings& SetZoom(float zoom)                { Zoom = zoom; return *this; }
};

class OrthoCamera : public Camera
{
public:
    OrthoCamera(const OrthoCameraSettings& settings = OrthoCameraSettings());
    ~OrthoCamera() override = default;

    void SetPosition(const glm::vec3& pos);
    const glm::vec3& GetPosition() const override { return m_position; }

    void SetDirection(const glm::vec3& direction) override;
    void LookAt(const glm::vec3& point) override;
    const glm::vec3& GetDirection() const override { return m_direction; }

    void SetUp(const glm::vec3& up) override;
    const glm::vec3& GetUp() const override { return m_up; }

    void SetBoundaries(const OrthoCameraBoundaries& boundaries);
    const OrthoCameraBoundaries& GetBoundaries() const { return m_boundaries; }

    void SetAspectRatio(float aspectRatio) override;
    float GetAspectRatio() const override { return m_aspectRatio; }

    void SetZoom(float zoom);
    float GetZoom() const { return m_zoom; }

    const glm::mat4& GetViewMatrix() const override { return m_view; }
    const glm::mat4& GetProjectionMatrix() const override { return m_projection; }
    const glm::mat4& GetViewProjectionMatrix() const override { return m_viewProjection; }

private:
    glm::mat4 m_view;
    glm::mat4 m_projection;
    glm::mat4 m_viewProjection;

    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_up;

    OrthoCameraBoundaries m_boundaries;
    float m_aspectRatio;
    float m_zoom;

private:
    void RecalculateViewMatrix();
    void RecalculateProjectionMatrix();
    void RecalculateViewProjectionMatrix();
};

}

#endif
