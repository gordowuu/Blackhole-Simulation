#include "AccretionDisk.h"
#include "BlackHole.h"
#include "Constants.h"
#include <cmath>
#include <algorithm>

namespace Physics {

AccretionDisk::AccretionDisk(const BlackHole* blackHole)
    : m_blackHole(blackHole)
    , m_innerRadius(0.0f)
    , m_outerRadius(0.0f)
    , m_thickness(0.5f)
    , m_inclination(0.0f)
    , m_rotationSpeed(1.0f)
    , m_peakTemperature(100000.0f) {  // 100,000 K
    
    // Set inner radius to ISCO
    m_innerRadius = blackHole->getISCO();
    m_outerRadius = blackHole->getSchwarzschildRadius() * 15.0f;
}

float AccretionDisk::getTemperature(float radius) const {
    // Temperature profile: T(r) ∝ r^(-3/4) for thin disk
    // Normalize so temperature is peak at inner radius
    
    if (radius < m_innerRadius) {
        return 0.0f;
    }
    
    float ratio = m_innerRadius / radius;
    float temperature = m_peakTemperature * std::pow(ratio, 0.75f);
    
    // Add some variation for visual interest
    return temperature * (0.9f + 0.2f * std::sin(radius * 10.0f));
}

glm::vec3 AccretionDisk::getVelocity(float radius, float phi) const {
    // Keplerian orbital velocity: v = sqrt(GM/r)
    // In our units: v = sqrt(M/r)
    
    float M = m_blackHole->getSchwarzschildRadius() * 0.5f;
    float v = std::sqrt(M / radius) * m_rotationSpeed;
    
    // Velocity is tangential (in phi direction)
    return glm::vec3(-std::sin(phi) * v, 0.0f, std::cos(phi) * v);
}

glm::vec3 AccretionDisk::getEmission(float radius, float temperature) const {
    // Convert temperature to color using blackbody approximation
    glm::vec3 color = temperatureToRGB(temperature);
    
    // Intensity falls off with radius
    float intensity = std::pow(m_innerRadius / radius, 2.0f);
    intensity = std::clamp(intensity, 0.0f, 10.0f);
    
    return color * intensity;
}

float AccretionDisk::getDopplerFactor(const glm::vec3& position, const glm::vec3& observerDir) const {
    // Calculate Doppler factor for relativistic motion
    
    float radius = glm::length(glm::vec2(position.x, position.z));
    if (radius < m_innerRadius || radius > m_outerRadius) {
        return 1.0f;
    }
    
    float phi = std::atan2(position.z, position.x);
    glm::vec3 velocity = getVelocity(radius, phi);
    
    float beta = glm::length(velocity) / SPEED_OF_LIGHT;
    beta = std::min(beta, 0.99f);  // Cap at 99% speed of light
    
    // Direction of motion
    glm::vec3 motionDir = glm::normalize(velocity);
    
    // Doppler factor: δ = 1 / [γ(1 - β·cos(θ))]
    float gamma = 1.0f / std::sqrt(1.0f - beta * beta);
    float cosTheta = glm::dot(motionDir, observerDir);
    
    float doppler = 1.0f / (gamma * (1.0f - beta * cosTheta));
    
    return std::clamp(doppler, 0.1f, 10.0f);
}

bool AccretionDisk::intersectRay(const glm::vec3& origin, const glm::vec3& direction, 
                                  float& t, float& radius, float& phi) const {
    // Disk is in XZ plane (y = 0)
    // Ray equation: P = origin + t * direction
    
    // Check if ray is parallel to disk
    if (std::abs(direction.y) < 1e-6f) {
        return false;
    }
    
    // Solve for t when y = 0
    t = -origin.y / direction.y;
    
    if (t < 0.0f) {
        return false;
    }
    
    // Intersection point
    glm::vec3 hitPoint = origin + t * direction;
    
    // Check if within disk radii
    radius = glm::length(glm::vec2(hitPoint.x, hitPoint.z));
    
    if (radius < m_innerRadius || radius > m_outerRadius) {
        return false;
    }
    
    // Check if within disk thickness
    if (std::abs(hitPoint.y) > m_thickness) {
        return false;
    }
    
    // Calculate azimuthal angle
    phi = std::atan2(hitPoint.z, hitPoint.x);
    
    return true;
}

} // namespace Physics
