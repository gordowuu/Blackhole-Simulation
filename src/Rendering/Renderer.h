#pragma once

#include <memory>
#include <glm/glm.hpp>

namespace Core {
    class Shader;
    class Camera;
}

namespace Physics {
    class BlackHole;
    class AccretionDisk;
}

namespace Rendering {
    class Texture;
    class PostProcess;
}

namespace Rendering {

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();
    
    void initialize();
    void resize(int width, int height);
    
    // Main rendering function
    void render(const Core::Camera& camera, 
                const Physics::BlackHole& blackHole,
                const Physics::AccretionDisk& disk);
    
    // Settings
    void setQuality(int quality);
    void setEnableBloom(bool enable) { m_enableBloom = enable; }
    void setExposure(float exposure) { m_exposure = exposure; }
    void setShowEventHorizon(bool show) { m_showEventHorizon = show; }
    void setShowPhotonSphere(bool show) { m_showPhotonSphere = show; }
    void setShowAccretionDisk(bool show) { m_showAccretionDisk = show; }
    
    // Getters
    int getQuality() const { return m_quality; }
    const char* getQualityName() const;
    bool getEnableBloom() const { return m_enableBloom; }
    float getExposure() const { return m_exposure; }
    bool getShowEventHorizon() const { return m_showEventHorizon; }
    bool getShowPhotonSphere() const { return m_showPhotonSphere; }
    bool getShowAccretionDisk() const { return m_showAccretionDisk; }
    
private:
    void createFullscreenQuad();
    void loadShaders();
    void generateStarfield();
    
    int m_width;
    int m_height;
    int m_quality;  // 1 = low, 2 = medium, 3 = high
    
    // Rendering options
    bool m_enableBloom;
    bool m_showEventHorizon;
    bool m_showPhotonSphere;
    bool m_showAccretionDisk;
    float m_exposure;
    
    // OpenGL objects
    unsigned int m_quadVAO;
    unsigned int m_quadVBO;
    
    // Shaders
    std::unique_ptr<Core::Shader> m_rayTracerShader;
    std::unique_ptr<Core::Shader> m_displayShader;
    std::unique_ptr<Core::Shader> m_postProcessShader;
    
    // Textures
    std::unique_ptr<Texture> m_outputTexture;
    std::unique_ptr<Texture> m_starfieldTexture;
    
    // Post-processing
    std::unique_ptr<PostProcess> m_postProcess;
};

} // namespace Rendering
