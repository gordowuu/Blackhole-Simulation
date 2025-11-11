#pragma once

#include <glm/glm.hpp>

namespace Physics {

class BlackHole;

class AccretionDisk {
public:
    AccretionDisk(const BlackHole* blackHole);
    
    // Getters
    float getInnerRadius() const { return m_innerRadius; }
    float getOuterRadius() const { return m_outerRadius; }
    float getThickness() const { return m_thickness; }
    float getInclination() const { return m_inclination; }
    float getRotationSpeed() const { return m_rotationSpeed; }
    
    // Setters
    void setInnerRadius(float radius) { m_innerRadius = radius; }
    void setOuterRadius(float radius) { m_outerRadius = radius; }
    void setThickness(float thickness) { m_thickness = thickness; }
    void setInclination(float inclination) { m_inclination = inclination; }
    void setRotationSpeed(float speed) { m_rotationSpeed = speed; }
    
    // Physics calculations
    float getTemperature(float radius) const;
    glm::vec3 getVelocity(float radius, float phi) const;
    glm::vec3 getEmission(float radius, float temperature) const;
    
    // Doppler shift at given position
    float getDopplerFactor(const glm::vec3& position, const glm::vec3& observerDir) const;
    
    // Check if ray intersects disk
    bool intersectRay(const glm::vec3& origin, const glm::vec3& direction, 
                      float& t, float& radius, float& phi) const;

private:
    const BlackHole* m_blackHole;
    
    float m_innerRadius;      // Usually at ISCO
    float m_outerRadius;      
    float m_thickness;        // Vertical thickness
    float m_inclination;      // Disk tilt angle (radians)
    float m_rotationSpeed;    // Angular velocity factor
    float m_peakTemperature;  // Temperature at inner edge
};

} // namespace Physics
