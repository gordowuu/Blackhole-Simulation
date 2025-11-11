#pragma once

#include <glm/glm.hpp>

namespace Core {

enum class CameraMode {
    Orbit,
    FreeFly
};

class Camera {
public:
    Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 10.0f),
           const glm::vec3& target = glm::vec3(0.0f, 0.0f, 0.0f),
           float fov = 60.0f);
    
    // Update camera
    void update(float deltaTime);
    
    // Get matrices
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;
    
    // Camera controls
    void setMode(CameraMode mode) { m_mode = mode; }
    void setPosition(const glm::vec3& position) { m_position = position; }
    void setTarget(const glm::vec3& target) { m_target = target; }
    void setFOV(float fov) { m_fov = fov; }
    void setDistance(float distance) { m_distance = distance; }
    
    // Orbit controls
    void orbit(float deltaTheta, float deltaPhi);
    void zoom(float delta);
    void pan(const glm::vec2& delta);
    
    // FreeFly controls
    void move(const glm::vec3& direction, float speed);
    void rotate(float deltaYaw, float deltaPitch);
    
    // Getters
    glm::vec3 getPosition() const { return m_position; }
    glm::vec3 getTarget() const { return m_target; }
    glm::vec3 getForward() const;
    glm::vec3 getRight() const;
    glm::vec3 getUp() const { return m_up; }
    float getFOV() const { return m_fov; }
    float getDistance() const { return m_distance; }
    
    // Orbit parameters
    float getTheta() const { return m_theta; }
    float getPhi() const { return m_phi; }
    
private:
    void updateOrbitPosition();
    void updateVectors();
    
    CameraMode m_mode;
    
    // Camera parameters
    glm::vec3 m_position;
    glm::vec3 m_target;
    glm::vec3 m_up;
    glm::vec3 m_forward;
    glm::vec3 m_right;
    
    float m_fov;
    float m_nearPlane;
    float m_farPlane;
    
    // Orbit mode parameters
    float m_distance;
    float m_theta;  // Azimuthal angle
    float m_phi;    // Polar angle
    
    // FreeFly mode parameters
    float m_yaw;
    float m_pitch;
};

} // namespace Core
