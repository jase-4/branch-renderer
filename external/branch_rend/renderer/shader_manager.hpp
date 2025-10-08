#ifndef SHADER_MANAGER_HPP
#define  SHADER_MANAGER_HPP

#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
#include <cassert>  

#include "shader.hpp"

class ShaderManager {
public:
    static ShaderManager& getInstance();
    std::shared_ptr<Shader> get_shader(const std::string& shaderName);
    void load_shader(const std::string& shaderName, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

private:
    ShaderManager();
    ~ShaderManager() = default;

    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
    
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;
};

#endif
