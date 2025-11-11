#include "Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace Core {

Shader::~Shader() {
    if (m_program) {
        glDeleteProgram(m_program);
    }
}

bool Shader::loadFromFile(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexSource = readFile(vertexPath);
    std::string fragmentSource = readFile(fragmentPath);
    
    if (vertexSource.empty() || fragmentSource.empty()) {
        std::cerr << "Failed to read shader files" << std::endl;
        return false;
    }
    
    unsigned int vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
    
    if (vertexShader == 0 || fragmentShader == 0) {
        return false;
    }
    
    bool success = linkProgram(vertexShader, fragmentShader);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return success;
}

bool Shader::loadComputeShader(const std::string& computePath) {
    std::string computeSource = readFile(computePath);
    
    if (computeSource.empty()) {
        std::cerr << "Failed to read compute shader file" << std::endl;
        return false;
    }
    
    unsigned int computeShader = compileShader(computeSource, GL_COMPUTE_SHADER);
    
    if (computeShader == 0) {
        return false;
    }
    
    bool success = linkComputeProgram(computeShader);
    
    glDeleteShader(computeShader);
    
    return success;
}

void Shader::use() const {
    glUseProgram(m_program);
}

void Shader::dispatch(unsigned int numGroupsX, unsigned int numGroupsY, unsigned int numGroupsZ) const {
    glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
}

unsigned int Shader::compileShader(const std::string& source, unsigned int type) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::cerr << "Shader compilation error (" 
                  << (type == GL_VERTEX_SHADER ? "Vertex" : 
                      type == GL_FRAGMENT_SHADER ? "Fragment" : "Compute")
                  << "):\n" << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

bool Shader::linkProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    m_program = glCreateProgram();
    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);
    glLinkProgram(m_program);
    
    int success;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(m_program, 1024, nullptr, infoLog);
        std::cerr << "Program linking error:\n" << infoLog << std::endl;
        glDeleteProgram(m_program);
        m_program = 0;
        return false;
    }
    
    return true;
}

bool Shader::linkComputeProgram(unsigned int computeShader) {
    m_program = glCreateProgram();
    glAttachShader(m_program, computeShader);
    glLinkProgram(m_program);
    
    int success;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(m_program, 1024, nullptr, infoLog);
        std::cerr << "Compute program linking error:\n" << infoLog << std::endl;
        glDeleteProgram(m_program);
        m_program = 0;
        return false;
    }
    
    return true;
}

std::string Shader::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int Shader::getUniformLocation(const std::string& name) {
    if (m_uniformCache.find(name) != m_uniformCache.end()) {
        return m_uniformCache[name];
    }
    
    int location = glGetUniformLocation(m_program, name.c_str());
    m_uniformCache[name] = location;
    return location;
}

void Shader::setBool(const std::string& name, bool value) {
    glUniform1i(getUniformLocation(name), static_cast<int>(value));
}

void Shader::setInt(const std::string& name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) {
    glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) {
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setMat3(const std::string& name, const glm::mat3& value) {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

} // namespace Core
