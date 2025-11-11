#pragma once

#include <string>

namespace Rendering {

class Texture {
public:
    Texture();
    ~Texture();
    
    // Load texture from file
    bool loadFromFile(const std::string& path, bool hdr = false);
    
    // Create empty texture
    bool create(int width, int height, int channels, bool hdr = false);
    
    // Bind texture
    void bind(unsigned int slot = 0) const;
    void unbind() const;
    
    // Bind as image for compute shaders
    void bindImage(unsigned int slot, unsigned int access) const;
    
    // Getters
    unsigned int getID() const { return m_textureID; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    int getChannels() const { return m_channels; }
    
private:
    unsigned int m_textureID;
    int m_width;
    int m_height;
    int m_channels;
    bool m_isHDR;
};

} // namespace Rendering
