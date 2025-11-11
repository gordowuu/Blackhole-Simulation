#include "PostProcess.h"
#include "Texture.h"
#include "../Core/Shader.h"
#include <glad/glad.h>
#include <iostream>

namespace Rendering {

PostProcess::PostProcess(int width, int height)
    : m_width(width)
    , m_height(height)
    , m_fbo(0)
    , m_rbo(0) {
    
    createFramebuffer();
    
    // Create ping-pong framebuffers for bloom
    glGenFramebuffers(2, m_pingpongFBO);
    glGenTextures(2, m_pingpongBuffers);
    
    for (unsigned int i = 0; i < 2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, m_pingpongBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pingpongBuffers[i], 0);
    }
}

PostProcess::~PostProcess() {
    deleteFramebuffer();
    glDeleteFramebuffers(2, m_pingpongFBO);
    glDeleteTextures(2, m_pingpongBuffers);
}

void PostProcess::resize(int width, int height) {
    m_width = width;
    m_height = height;
    
    deleteFramebuffer();
    createFramebuffer();
    
    // Resize ping-pong buffers
    for (unsigned int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, m_pingpongBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    }
}

void PostProcess::applyBloom(const Texture& input, Texture& output, float threshold, float intensity) {
    // Simple bloom implementation
    // This would require a proper shader - for now just placeholder
    // In real implementation:
    // 1. Extract bright pixels above threshold
    // 2. Blur bright pixels (Gaussian blur, ping-pong between buffers)
    // 3. Combine with original image
}

void PostProcess::applyToneMapping(const Texture& input, Texture& output, float exposure) {
    // Tone mapping would be done in shader
    // For now this is a placeholder
}

void PostProcess::createFramebuffer() {
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    
    // Create renderbuffer for depth and stencil
    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
    
    // Check framebuffer completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete!" << std::endl;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcess::deleteFramebuffer() {
    if (m_fbo) {
        glDeleteFramebuffers(1, &m_fbo);
        m_fbo = 0;
    }
    if (m_rbo) {
        glDeleteRenderbuffers(1, &m_rbo);
        m_rbo = 0;
    }
}

} // namespace Rendering
