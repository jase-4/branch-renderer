#include "shader_manager.hpp"

// Singleton instance getter
ShaderManager& ShaderManager::getInstance() {
    static ShaderManager instance;  // Instantiated once and persists
    return instance;
}

// Retrieve a shader by name
std::shared_ptr<Shader> ShaderManager::get_shader(const std::string& shaderName) {
    assert(shaders.find(shaderName) != shaders.end() && "Shader not found!");
    return shaders[shaderName];
}

// Load a shader into the manager
void ShaderManager::load_shader(const std::string& shaderName, const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
    shaders[shaderName] = std::make_shared<Shader>(vertexPath, fragmentPath, geometryPath);
}

// Private constructor to initialize shaders
ShaderManager::ShaderManager() {
    // Initialize shaders upfront
    shaders["depth_cubemap"] = std::make_shared<Shader>("shaders/shadows_depth.vs", "shaders/shadows_depth.fs", "shaders/shadows_depth.gs");
    shaders["depth_map2d"] = std::make_shared<Shader>("shaders/depth_map2d.vs", "shaders/depth_map2d.fs");
    shaders["aabb"] = std::make_shared<Shader>("shaders/aabb.vs", "shaders/aabb.fs");

    shaders["lighting_shadow"] = std::make_shared<Shader>("shaders/lighting_shadow.vs", "shaders/temp.fs");
    shaders["light_cube"] = std::make_shared<Shader>("shaders/shader2.vert", "shaders/shader2.frag");
    shaders["depth_quad"] = std::make_shared<Shader>("shaders/debug_quad.vs", "shaders/debug_quad.fs");
    shaders["shadow_map2"] = std::make_shared<Shader>("shaders/shadow_map2.vs", "shaders/shadow_map2.fs");
    // Add more shaders as needed
}
