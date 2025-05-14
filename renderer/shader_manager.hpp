#ifndef SHADER_MANAGER_HPP
#define  SHADER_MANAGER_HPP

#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
#include <cassert>  // For assert

#include "shader.hpp"

class ShaderManager {
public:
    // Singleton pattern
    static ShaderManager& getInstance();

    // Retrieve a shader by name
    std::shared_ptr<Shader> get_shader(const std::string& shaderName);

    // Load a shader into the manager
    void load_shader(const std::string& shaderName, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

private:
    // Private constructor to prevent instantiation
    ShaderManager();

    // Destructor
    ~ShaderManager() = default;

    // Shader storage map
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;

    // Delete copy constructor and copy assignment operator for Singleton pattern
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;
};

#endif
