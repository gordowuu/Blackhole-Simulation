#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <algorithm>

namespace Core {

Camera::Camera(const glm::vec3& position, const glm::vec3& target, float fov)
    : m_mode(CameraMode::Orbit)
    , m_position(position)
    , m_target(target)
    , m_up(glm::vec3(0.0f, 1.0f, 0.0f))
    , m_fov(fov)
    , m_nearPlane(0.1f)
    , m_farPlane(1000.0f)
    , m_distance(glm::length(position - target))
    , m_theta(0.0f)
    , m_phi(glm::half_pi<float>())
    , m_yaw(-90.0f)
    , m_pitch(0.0f) {
    
    updateVectors();
}

void Camera::update(float deltaTime) {
    if (m_mode == CameraMode::Orbit) {
        updateOrbitPosition();
    }
    updateVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(m_position, m_target, m_up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(m_fov), aspectRatio, m_nearPlane, m_farPlane);
}

void Camera::orbit(float deltaTheta, float deltaPhi) {
    m_theta += deltaTheta;
    m_phi += deltaPhi;
    
    // Clamp phi to avoid gimbal lock
    m_phi = std::clamp(m_phi, 0.01f, glm::pi<float>() - 0.01f);
    
    // Normalize theta
    while (m_theta > glm::two_pi<float>()) m_theta -= glm::two_pi<float>();
    while (m_theta < 0.0f) m_theta += glm::two_pi<float>();
    
    updateOrbitPosition();
}

void Camera::zoom(float delta) {
    m_distance += delta;
    m_distance = std::max(0.5f, m_distance);
    updateOrbitPosition();
}

void Camera::pan(const glm::vec2& delta) {
    glm::vec3 right = getRight();
    glm::vec3 up = getUp();
    
    m_target += right * delta.x + up * delta.y;
    updateOrbitPosition();
}

void Camera::move(const glm::vec3& direction, float speed) {
    m_position += direction * speed;
    m_target += direction * speed;
}

void Camera::rotate(float deltaYaw, float deltaPitch) {
    m_yaw += deltaYaw;
    m_pitch += deltaPitch;
    
    // Clamp pitch
    m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);
    
    updateVectors();
}

glm::vec3 Camera::getForward() const {
    return m_forward;
}

glm::vec3 Camera::getRight() const {
    return m_right;
}

void Camera::updateOrbitPosition() {
    // Spherical to Cartesian coordinates
    float x = m_distance * std::sin(m_phi) * std::cos(m_theta);
    float y = m_distance * std::cos(m_phi);
    float z = m_distance * std::sin(m_phi) * std::sin(m_theta);
    
    m_position = m_target + glm::vec3(x, y, z);
}

void Camera::updateVectors() {
    if (m_mode == CameraMode::Orbit) {
        m_forward = glm::normalize(m_target - m_position);
    } else {
        // FreeFly mode
        glm::vec3 front;
        front.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
        front.y = std::sin(glm::radians(m_pitch));
        front.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
        m_forward = glm::normalize(front);
        m_target = m_position + m_forward;
    }
    
    m_right = glm::normalize(glm::cross(m_forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    m_up = glm::normalize(glm::cross(m_right, m_forward));
}

} // namespace Core
