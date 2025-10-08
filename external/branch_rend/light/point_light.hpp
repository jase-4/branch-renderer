#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "light.hpp"
#include "renderer/shader_manager.hpp"
#include <memory>
#include <vector>
#include <glm/glm.hpp>

class DepthMap;
class DepthMapCubeMap;

class PointLight : public Light
{
private:
    glm::vec3 light_pos = glm::vec3(-2.0f, 1.0f, 2.0f);
    float constant = 1.0f;
    float linear = 0.009f;
    float quadratic = 0.00032f;
    glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    std::unique_ptr<DepthMap> depth_map;
    unsigned int shadow_width = 1024;
    unsigned int shadow_height = 1024;
    glm::mat4 shadow_projection = glm::perspective(glm::radians(90.0f), 
                                                   (float)shadow_width / (float)shadow_height, 
                                                   0.1f, 35.0f);
    float near_plane = 0.1f;
    float far_plane = 35.0f;
    bool shadows = false;
    std::shared_ptr<Shader> depth_shader = ShaderManager::getInstance().get_shader("depth_cubemap");

public:
    PointLight();
    ~PointLight();

    void init_depth_map();
    std::shared_ptr<Shader> get_depth_shader() const override;
    void set_light_pos(glm::vec3 new_pos) override;
    unsigned int get_shadow_width() const override;
    unsigned int get_shadow_height() const override;
    float get_far_plane() const override;
    float get_near_plane() const override;
    GLuint get_depth_map_FBO() const override;
    unsigned int get_depth_map_texture() const override;
    std::vector<glm::mat4> gen_cubemap_transform();
    void set_light_shad_shader_vars(std::shared_ptr<Shader> shader) override;
    void print_glm(glm::vec3 pos);
    void set_depth_map_shader() override;
    glm::vec3 get_light_pos() const override;
    void bind_shadow_map_texture() const override;
    void set_light_uniforms(std::shared_ptr<Shader> shader) override;
};

#endif // POINT_LIGHT_H
