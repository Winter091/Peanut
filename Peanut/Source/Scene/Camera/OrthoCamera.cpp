#include <Peanut/Scene/Camera/OrthoCamera.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <Peanut/Core/Assert.hpp>

namespace pn {

OrthoCamera::OrthoCamera(const OrthoCameraSettings& settings)
    : m_position(settings.Position)
    , m_direction(glm::normalize(settings.Direction))
    , m_up(glm::normalize(settings.Up))
    , m_boundaries(settings.Boundaries)
    , m_aspectRatio(settings.AspectRatio)
    , m_zoom(settings.Zoom)
{
    RecalculateViewMatrix();
    RecalculateProjectionMatrix();
}

void OrthoCamera::SetPosition(const glm::vec3& pos)
{
    m_position = pos;
    RecalculateViewMatrix();
}

void OrthoCamera::SetDirection(const glm::vec3& direction)
{
    m_direction = glm::normalize(direction);
    RecalculateViewMatrix();
}

void OrthoCamera::LookAt(const glm::vec3& point)
{
    SetDirection(point - m_position);
}

void OrthoCamera::SetUp(const glm::vec3& up)
{
    m_up = glm::normalize(up);
    RecalculateViewMatrix();
}

void OrthoCamera::SetBoundaries(const OrthoCameraBoundaries& boundaries)
{
    m_boundaries = boundaries;
    RecalculateProjectionMatrix();
}

void OrthoCamera::SetAspectRatio(float aspectRatio)
{
    m_aspectRatio = aspectRatio;
    RecalculateProjectionMatrix();
}

void OrthoCamera::SetZoom(float zoom)
{
    m_zoom = zoom;
    RecalculateProjectionMatrix();   
}

void OrthoCamera::RecalculateViewMatrix()
{
    m_view = glm::lookAt(m_position, m_position + m_direction, m_up);
    RecalculateViewProjectionMatrix();
}

void OrthoCamera::RecalculateProjectionMatrix()
{
    m_projection = glm::ortho(m_boundaries.Left * m_aspectRatio * m_zoom, m_boundaries.Right * m_aspectRatio * m_zoom,
                              m_boundaries.Bottom * m_zoom, m_boundaries.Top * m_zoom, 
                              m_boundaries.Near, m_boundaries.Far
    );
    RecalculateViewProjectionMatrix();
}

void OrthoCamera::RecalculateViewProjectionMatrix()
{
    m_viewProjection = m_projection * m_view;
}

}