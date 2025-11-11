#pragma once

#include <glm/glm.hpp>

namespace Physics {

// Physical constants
constexpr float SPEED_OF_LIGHT = 299792458.0f;           // m/s
constexpr float GRAVITATIONAL_CONSTANT = 6.67430e-11f;   // m^3 kg^-1 s^-2
constexpr float SOLAR_MASS = 1.98892e30f;                // kg
constexpr float PLANCK_CONSTANT = 6.62607015e-34f;       // J⋅s
constexpr float BOLTZMANN_CONSTANT = 1.380649e-23f;      // J/K
constexpr float STEFAN_BOLTZMANN = 5.670374419e-8f;      // W⋅m^-2⋅K^-4

// Simulation units (geometric units where G = c = 1)
constexpr float UNIT_MASS = 1.0f;                         // In solar masses
constexpr float UNIT_LENGTH = 1.0f;                       // In Schwarzschild radii
constexpr float UNIT_TIME = 1.0f;                         // In GM/c^3

// Conversion factors
inline float schwarzschildRadius(float massInSolarMasses) {
    return 2.0f * GRAVITATIONAL_CONSTANT * massInSolarMasses * SOLAR_MASS 
           / (SPEED_OF_LIGHT * SPEED_OF_LIGHT);
}

// Black hole parameters
constexpr float DEFAULT_MASS = 4.31e6f;                   // M87 black hole in solar masses
constexpr float MIN_SPIN = 0.0f;                          // Non-rotating (Schwarzschild)
constexpr float MAX_SPIN = 0.998f;                        // Near-maximal rotation

// Accretion disk parameters
constexpr float DISK_INNER_RADIUS_FACTOR = 3.0f;          // ISCO for Schwarzschild
constexpr float DISK_OUTER_RADIUS_FACTOR = 20.0f;
constexpr float DISK_THICKNESS_FACTOR = 0.1f;
constexpr float DISK_TEMPERATURE_K = 1e5f;                // Peak temperature in Kelvin

// Ray tracing parameters
constexpr int MAX_RAY_STEPS = 2000;
constexpr float RAY_STEP_SIZE = 0.02f;
constexpr float RAY_MIN_RADIUS = 0.5f;                    // Stop if too close to singularity
constexpr float RAY_MAX_RADIUS = 1000.0f;                 // Stop if escaped
constexpr float EPSILON = 1e-6f;

// Rendering parameters
constexpr int DEFAULT_WIDTH = 1920;
constexpr int DEFAULT_HEIGHT = 1080;
constexpr float DEFAULT_FOV = 60.0f;
constexpr float NEAR_PLANE = 0.1f;
constexpr float FAR_PLANE = 1000.0f;

// Color temperature to RGB conversion
inline glm::vec3 temperatureToRGB(float temperature) {
    // Simplified blackbody color approximation
    temperature = glm::clamp(temperature / 1000.0f, 1.0f, 40.0f);
    
    glm::vec3 color;
    
    // Red
    if (temperature <= 66.0f) {
        color.r = 1.0f;
    } else {
        float t = temperature - 60.0f;
        color.r = glm::clamp(1.292936186f * glm::pow(t, -0.1332047592f), 0.0f, 1.0f);
    }
    
    // Green
    if (temperature <= 66.0f) {
        color.g = glm::clamp(0.39008157876f * glm::log(temperature) - 0.63184144378f, 0.0f, 1.0f);
    } else {
        float t = temperature - 60.0f;
        color.g = glm::clamp(1.129890861f * glm::pow(t, -0.0755148492f), 0.0f, 1.0f);
    }
    
    // Blue
    if (temperature >= 66.0f) {
        color.b = 1.0f;
    } else if (temperature <= 19.0f) {
        color.b = 0.0f;
    } else {
        float t = temperature - 10.0f;
        color.b = glm::clamp(0.54320678911f * glm::log(t) - 1.19625408914f, 0.0f, 1.0f);
    }
    
    return color;
}

// Doppler shift factor calculation
inline float dopplerFactor(float velocity, float cosTheta) {
    float beta = velocity / SPEED_OF_LIGHT;
    float gamma = 1.0f / glm::sqrt(1.0f - beta * beta);
    return 1.0f / (gamma * (1.0f - beta * cosTheta));
}

} // namespace Physics
