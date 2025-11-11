#include "Texture.h"
#include <glad/glad.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Rendering {

Texture::Texture()
    : m_textureID(0)
    , m_width(0)
    , m_height(0)
    , m_channels(0)
    , m_isHDR(false) {
}

Texture::~Texture() {
    if (m_textureID) {
        glDeleteTextures(1, &m_textureID);
    }
}

bool Texture::loadFromFile(const std::string& path, bool hdr) {
    m_isHDR = hdr;
    
    void* data = nullptr;
    
    if (hdr) {
        stbi_set_flip_vertically_on_load(false);
        data = stbi_loadf(path.c_str(), &m_width, &m_height, &m_channels, 0);
    } else {
        stbi_set_flip_vertically_on_load(true);
        data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
    }
    
    if (!data) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        std::cerr << "STB Error: " << stbi_failure_reason() << std::endl;
        return false;
    }
    
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Upload texture data
    if (hdr) {
        GLenum format = (m_channels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_width, m_height, 0, format, GL_FLOAT, data);
    } else {
        GLenum format = (m_channels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
    }
    
    glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(data);
    
    std::cout << "Loaded texture: " << path << " (" << m_width << "x" << m_height 
              << ", " << m_channels << " channels)" << std::endl;
    
    return true;
}

bool Texture::create(int width, int height, int channels, bool hdr) {
    m_width = width;
    m_height = height;
    m_channels = channels;
    m_isHDR = hdr;
    
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Create empty texture
    if (hdr) {
        GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;
        GLenum internalFormat = (channels == 3) ? GL_RGB16F : GL_RGBA16F;
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, nullptr);
    } else {
        GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
    }
    
    return true;
}

void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bindImage(unsigned int slot, unsigned int access) const {
    GLenum format = m_isHDR ? GL_RGBA16F : GL_RGBA8;
    glBindImageTexture(slot, m_textureID, 0, GL_FALSE, 0, access, format);
}

} // namespace Rendering
