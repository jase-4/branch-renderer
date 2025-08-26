#ifndef DIR_LIGHT_HPP
#define DIR_LIGHT_HPP

#include "light.hpp"
#include "renderer/shader_manager.hpp"
#include <memory>
#include <glm/glm.hpp>


class DepthMap;
class DepthMap2D;

class DirLight : public Light
{
private:
    glm::vec3 light_pos = glm::vec3(30.0f, 20.0f, 10.0f);
    glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    glm::vec3 diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
    glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);

    std::unique_ptr<DepthMap> depth_map;
    float near_plane = 0.1f;
    float far_plane = 150.0f;
    glm::vec3 targetPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    unsigned int shadow_width = 1024;
    unsigned int shadow_height = 1024;

    glm::mat4 light_projection = glm::perspective(glm::radians(45.0f), (GLfloat)shadow_width / (GLfloat)shadow_height, near_plane, far_plane);
    bool shadows = false;
    std::shared_ptr<Shader> depth_shader = ShaderManager::getInstance().get_shader("depth_map2d");

public:
    DirLight();
    ~DirLight();

    void init_depth_map();
    std::shared_ptr<Shader> get_depth_shader() const override;
    void set_light_pos(glm::vec3 new_pos) override;
    unsigned int get_shadow_width() const override;
    unsigned int get_shadow_height() const override;
    float get_far_plane() const override;
    float get_near_plane() const override;
    GLuint get_depth_map_FBO() const override;
    unsigned int get_depth_map_texture() const override;
    void set_light_shad_shader_vars(std::shared_ptr<Shader> shader) override;
    void set_depth_map_shader() override;
    glm::vec3 get_light_pos() const override;
    void bind_shadow_map_texture() const override;
    void set_light_uniforms(std::shared_ptr<Shader> shader) override;
};

#endif
