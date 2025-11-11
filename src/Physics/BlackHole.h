#pragma once

#include <glm/glm.hpp>

namespace Physics {

class BlackHole {
public:
    BlackHole(float mass = 1.0f, float spin = 0.0f);
    
    // Getters
    float getMass() const { return m_mass; }
    float getSpin() const { return m_spin; }
    glm::vec3 getPosition() const { return m_position; }
    
    // Setters
    void setMass(float mass);
    void setSpin(float spin);
    void setPosition(const glm::vec3& position) { m_position = position; }
    
    // Schwarzschild radius
    float getSchwarzschildRadius() const { return m_schwarzschildRadius; }
    
    // ISCO (Innermost Stable Circular Orbit)
    float getISCO() const;
    
    // Photon sphere radius
    float getPhotonSphereRadius() const;
    
    // Ergosphere radius (for Kerr black holes)
    float getErgosphereRadius(float theta) const;
    
    // Event horizon radius (depends on spin for Kerr)
    float getEventHorizonRadius(float theta = 0.0f) const;
    
    // Frame dragging angular velocity
    float getFrameDraggingVelocity(float r, float theta) const;
    
private:
    void updateDerivedQuantities();
    
    float m_mass;                    // In solar masses
    float m_spin;                    // Dimensionless spin parameter (0-1)
    glm::vec3 m_position;
    
    // Derived quantities
    float m_schwarzschildRadius;     // 2GM/c^2
    float m_spinParameter;           // a = J/Mc (in geometric units)
};

} // namespace Physics
