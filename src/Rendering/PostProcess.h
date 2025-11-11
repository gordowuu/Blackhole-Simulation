#pragma once

namespace Rendering {

class Texture;
class Shader;

class PostProcess {
public:
    PostProcess(int width, int height);
    ~PostProcess();
    
    void resize(int width, int height);
    
    // Apply post-processing effects
    void applyBloom(const Texture& input, Texture& output, float threshold, float intensity);
    void applyToneMapping(const Texture& input, Texture& output, float exposure);
    
    // Get framebuffer for rendering
    unsigned int getFramebuffer() const { return m_fbo; }
    
private:
    void createFramebuffer();
    void deleteFramebuffer();
    
    int m_width;
    int m_height;
    
    unsigned int m_fbo;
    unsigned int m_rbo;
    
    // Bloom buffers
    unsigned int m_pingpongFBO[2];
    unsigned int m_pingpongBuffers[2];
};

} // namespace Rendering
