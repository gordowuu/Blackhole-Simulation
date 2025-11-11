#include "BlackHole.h"
#include "Constants.h"
#include <cmath>
#include <algorithm>

namespace Physics {

BlackHole::BlackHole(float mass, float spin)
    : m_mass(mass)
    , m_spin(std::clamp(spin, MIN_SPIN, MAX_SPIN))
    , m_position(0.0f, 0.0f, 0.0f) {
    
    updateDerivedQuantities();
}

void BlackHole::setMass(float mass) {
    m_mass = std::max(0.1f, mass);
    updateDerivedQuantities();
}

void BlackHole::setSpin(float spin) {
    m_spin = std::clamp(spin, MIN_SPIN, MAX_SPIN);
    updateDerivedQuantities();
}

float BlackHole::getISCO() const {
    // ISCO radius depends on spin
    // For Schwarzschild (spin=0): r_ISCO = 6M
    // For maximal Kerr (spin=1): r_ISCO = M (prograde) or 9M (retrograde)
    
    float Z1 = 1.0f + std::pow(1.0f - m_spin * m_spin, 1.0f/3.0f) *
               (std::pow(1.0f + m_spin, 1.0f/3.0f) + std::pow(1.0f - m_spin, 1.0f/3.0f));
    float Z2 = std::sqrt(3.0f * m_spin * m_spin + Z1 * Z1);
    
    // Prograde orbit ISCO
    float rISCO = m_schwarzschildRadius * 0.5f * (3.0f + Z2 - std::sqrt((3.0f - Z1) * (3.0f + Z1 + 2.0f * Z2)));
    
    return rISCO;
}

float BlackHole::getPhotonSphereRadius() const {
    // For Schwarzschild: r = 1.5 * Rs
    // For Kerr, it's more complex and depends on spin
    if (m_spin < 0.01f) {
        return 1.5f * m_schwarzschildRadius;
    }
    
    // Approximate for Kerr (prograde)
    float a = m_spinParameter;
    return m_schwarzschildRadius * (1.0f + std::cos(2.0f/3.0f * std::acos(-a)));
}

float BlackHole::getErgosphereRadius(float theta) const {
    // Ergosphere exists only for rotating black holes
    if (m_spin < 0.01f) {
        return m_schwarzschildRadius;
    }
    
    // r_ergo = M + sqrt(M^2 - a^2 * cos^2(theta))
    float M = m_schwarzschildRadius * 0.5f;  // In geometric units
    float a = m_spinParameter;
    float cosTheta = std::cos(theta);
    
    return M + std::sqrt(M * M - a * a * cosTheta * cosTheta);
}

float BlackHole::getEventHorizonRadius(float theta) const {
    // For Schwarzschild: r = Rs
    // For Kerr: r+ = M + sqrt(M^2 - a^2)
    if (m_spin < 0.01f) {
        return m_schwarzschildRadius;
    }
    
    float M = m_schwarzschildRadius * 0.5f;
    float a = m_spinParameter;
    
    return M + std::sqrt(M * M - a * a);
}

float BlackHole::getFrameDraggingVelocity(float r, float theta) const {
    // Frame dragging angular velocity (Lense-Thirring effect)
    // omega = 2*M*a*r / (r^4 + a^2*r^2 + 2*M*a^2*r)
    
    if (m_spin < 0.01f) {
        return 0.0f;
    }
    
    float M = m_schwarzschildRadius * 0.5f;
    float a = m_spinParameter;
    
    float r2 = r * r;
    float a2 = a * a;
    
    float numerator = 2.0f * M * a * r;
    float denominator = r2 * r2 + a2 * r2 + 2.0f * M * a2 * r;
    
    if (denominator < 1e-6f) {
        return 0.0f;
    }
    
    return numerator / denominator;
}

void BlackHole::updateDerivedQuantities() {
    // Calculate Schwarzschild radius in NORMALIZED units for visualization
    // Rs = 2GM/c^2, but we normalize for reasonable visualization scale
    // 
    // Use logarithmic scaling to handle huge mass differences:
    // - Stellar mass (5-10 M☉) → Rs ≈ 1.5-2.0
    // - Supermassive (1e6-1e10 M☉) → Rs ≈ 3.0-5.0
    // This keeps all black holes visible but shows mass differences
    
    // Logarithmic scaling: Rs = base + scale * log10(mass)
    float baseMass = 10.0f;  // Reference mass
    float baseRadius = 2.0f;  // Base Schwarzschild radius
    float scale = 0.4f;       // Logarithmic scale factor
    
    if (m_mass > baseMass) {
        // log10(x) = log(x) / log(10)
        m_schwarzschildRadius = baseRadius + scale * std::log(m_mass / baseMass) / std::log(10.0f);
    } else {
        // For masses below reference, scale linearly to avoid negative Rs
        m_schwarzschildRadius = baseRadius * (m_mass / baseMass);
    }
    
    // Ensure minimum radius for visibility
    m_schwarzschildRadius = std::max(m_schwarzschildRadius, 1.0f);
    
    // Spin parameter a = J/Mc in geometric units
    // a is in range [0, M] where M = Rs/2
    m_spinParameter = m_spin * m_schwarzschildRadius * 0.5f;
}

} // namespace Physics
