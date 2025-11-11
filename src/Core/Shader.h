#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Core {

class Shader {
public:
    Shader() : m_program(0) {}
    ~Shader();

    // Load and compile shaders
    bool loadFromFile(const std::string& vertexPath, const std::string& fragmentPath);
    bool loadComputeShader(const std::string& computePath);
    
    // Use the shader
    void use() const;
    void dispatch(unsigned int numGroupsX, unsigned int numGroupsY, unsigned int numGroupsZ) const;
    
    // Utility functions
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec4(const std::string& name, const glm::vec4& value);
    void setMat3(const std::string& name, const glm::mat3& value);
    void setMat4(const std::string& name, const glm::mat4& value);
    
    unsigned int getProgram() const { return m_program; }

private:
    unsigned int compileShader(const std::string& source, unsigned int type);
    bool linkProgram(unsigned int vertexShader, unsigned int fragmentShader);
    bool linkComputeProgram(unsigned int computeShader);
    std::string readFile(const std::string& path);
    int getUniformLocation(const std::string& name);
    
    unsigned int m_program;
    std::unordered_map<std::string, int> m_uniformCache;
};

} // namespace Core
