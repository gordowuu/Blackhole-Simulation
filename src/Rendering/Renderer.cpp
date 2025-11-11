#include "Renderer.h"
#include "Texture.h"
#include "PostProcess.h"
#include "../Core/Shader.h"
#include "../Core/Camera.h"
#include "../Physics/BlackHole.h"
#include "../Physics/AccretionDisk.h"
#include <glad/glad.h>
#include <iostream>
#include <random>

namespace Rendering {

Renderer::Renderer(int width, int height)
    : m_width(width)
    , m_height(height)
    , m_quality(2)
    , m_enableBloom(true)
    , m_showEventHorizon(true)
    , m_showPhotonSphere(false)
    , m_showAccretionDisk(true)
    , m_exposure(1.0f)
    , m_quadVAO(0)
    , m_quadVBO(0) {
}

Renderer::~Renderer() {
    if (m_quadVAO) {
        glDeleteVertexArrays(1, &m_quadVAO);
    }
    if (m_quadVBO) {
        glDeleteBuffers(1, &m_quadVBO);
    }
}

void Renderer::initialize() {
    createFullscreenQuad();
    loadShaders();  // Will throw exception if shaders fail
    generateStarfield();
    
    // Create output texture
    m_outputTexture = std::make_unique<Texture>();
    m_outputTexture->create(m_width, m_height, 4, true);  // RGBA HDR
    
    std::cout << "Created output texture: " << m_width << "x" << m_height << " RGBA16F" << std::endl;
    
    // Create post-processing
    m_postProcess = std::make_unique<PostProcess>(m_width, m_height);
    
    std::cout << "Renderer initialized" << std::endl;
}

void Renderer::resize(int width, int height) {
    m_width = width;
    m_height = height;
    
    m_outputTexture->create(width, height, 4, true);
    m_postProcess->resize(width, height);
}

void Renderer::render(const Core::Camera& camera, 
                       const Physics::BlackHole& blackHole,
                       const Physics::AccretionDisk& disk) {
    // Compute shader ray tracing pass
    if (m_rayTracerShader) {
        m_rayTracerShader->use();
        
        // Set uniforms
        m_rayTracerShader->setVec3("u_cameraPos", camera.getPosition());
        m_rayTracerShader->setVec3("u_cameraTarget", camera.getTarget());
        m_rayTracerShader->setVec3("u_cameraUp", camera.getUp());
        m_rayTracerShader->setFloat("u_fov", camera.getFOV());
        m_rayTracerShader->setFloat("u_aspectRatio", static_cast<float>(m_width) / m_height);
        
        // Black hole parameters
        m_rayTracerShader->setFloat("u_blackHoleMass", blackHole.getMass());
        m_rayTracerShader->setFloat("u_blackHoleSpin", blackHole.getSpin());
        m_rayTracerShader->setVec3("u_blackHolePos", blackHole.getPosition());
        m_rayTracerShader->setFloat("u_schwarzschildRadius", blackHole.getSchwarzschildRadius());
        
        // Accretion disk parameters
        m_rayTracerShader->setBool("u_showAccretionDisk", m_showAccretionDisk);
        m_rayTracerShader->setFloat("u_diskInnerRadius", disk.getInnerRadius());
        m_rayTracerShader->setFloat("u_diskOuterRadius", disk.getOuterRadius());
        m_rayTracerShader->setFloat("u_diskThickness", disk.getThickness());
        
        // Rendering options
        m_rayTracerShader->setBool("u_showEventHorizon", m_showEventHorizon);
        m_rayTracerShader->setBool("u_showPhotonSphere", m_showPhotonSphere);
        
        // Bind output texture as image
        m_outputTexture->bindImage(0, GL_WRITE_ONLY);
        
        // Bind starfield
        if (m_starfieldTexture) {
            m_starfieldTexture->bind(0);
            m_rayTracerShader->setInt("u_starfield", 0);
        }
        
        // Dispatch compute shader
        unsigned int workGroupsX = (m_width + 15) / 16;
        unsigned int workGroupsY = (m_height + 15) / 16;
        m_rayTracerShader->dispatch(workGroupsX, workGroupsY, 1);
        
        // Wait for compute shader to finish
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    }
    
    // Display pass
    // Don't clear! We want to draw on top of what ImGui might render
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (m_displayShader) {
        m_displayShader->use();
        m_displayShader->setFloat("u_exposure", m_exposure);
        
        m_outputTexture->bind(0);
        m_displayShader->setInt("u_texture", 0);
        
        glBindVertexArray(m_quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}

void Renderer::createFullscreenQuad() {
    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
        
        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    
    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &m_quadVBO);
    
    glBindVertexArray(m_quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    
    glBindVertexArray(0);
}

void Renderer::loadShaders() {
    bool success = true;
    
    // Load compute shader for ray tracing
    m_rayTracerShader = std::make_unique<Core::Shader>();
    if (!m_rayTracerShader->loadComputeShader("shaders/raytracer.comp")) {
        std::cerr << "Failed to load ray tracer compute shader" << std::endl;
        success = false;
    }
    
    // Load display shader
    m_displayShader = std::make_unique<Core::Shader>();
    if (!m_displayShader->loadFromFile("shaders/fullscreen.vert", "shaders/display.frag")) {
        std::cerr << "Failed to load display shader" << std::endl;
        success = false;
    }
    
    // Load post-processing shader
    m_postProcessShader = std::make_unique<Core::Shader>();
    if (!m_postProcessShader->loadFromFile("shaders/fullscreen.vert", "shaders/postprocess.frag")) {
        std::cerr << "Failed to load post-process shader" << std::endl;
        success = false;
    }
    
    if (!success) {
        throw std::runtime_error("One or more shaders failed to load");
    }
}

void Renderer::generateStarfield() {
    // Generate procedural starfield texture
    const int starfieldSize = 2048;
    const int numStars = 10000;
    
    std::vector<unsigned char> data(starfieldSize * starfieldSize * 3, 0);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> posDist(0, starfieldSize - 1);
    std::uniform_int_distribution<> brightDist(50, 255);
    
    // Generate random stars
    for (int i = 0; i < numStars; ++i) {
        int x = posDist(gen);
        int y = posDist(gen);
        int brightness = brightDist(gen);
        
        int idx = (y * starfieldSize + x) * 3;
        data[idx] = brightness;
        data[idx + 1] = brightness;
        data[idx + 2] = brightness;
        
        // Add some color variation
        if (brightness > 200) {
            // Blue stars
            data[idx + 2] = 255;
        } else if (brightness < 100) {
            // Red stars
            data[idx] = brightness + 50;
            data[idx + 2] = brightness / 2;
        }
    }
    
    m_starfieldTexture = std::make_unique<Texture>();
    // Would need to create texture from data - for now placeholder
    
    std::cout << "Generated starfield texture" << std::endl;
}

void Renderer::setQuality(int quality) {
    m_quality = quality;
    // Note: Quality changes would require recompiling shaders with different constants
    // For now, we just store the value - full implementation would need shader variants
}

const char* Renderer::getQualityName() const {
    switch (m_quality) {
        case 1: return "Low";
        case 2: return "Medium";
        case 3: return "High";
        case 4: return "Ultra";
        default: return "Unknown";
    }
}

} // namespace Rendering
