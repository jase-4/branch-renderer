#include "point_light.hpp"
#include "depth_map_cube.hpp"
#include <iostream>

PointLight::PointLight() {}

PointLight::~PointLight() {}

void PointLight::init_depth_map() {
    std::cout << shadow_width << std::endl;
    std::unique_ptr<DepthMapCubeMap> depth_map_2d = std::make_unique<DepthMapCubeMap>();
    depth_map = std::move(depth_map_2d);
    depth_map->init(shadow_width, shadow_height);
}

std::shared_ptr<Shader> PointLight::get_depth_shader() const {
    return depth_shader;
}

void PointLight::set_light_pos(glm::vec3 new_pos) {
    light_pos = new_pos;
}

unsigned int PointLight::get_shadow_width() const {
    return shadow_width;
}

unsigned int PointLight::get_shadow_height() const {
    return shadow_height;
}

float PointLight::get_far_plane() const {
    return far_plane;
}

float PointLight::get_near_plane() const {
    return near_plane;
}

GLuint PointLight::get_depth_map_FBO() const {
    return depth_map->get_fbo();
}

unsigned int PointLight::get_depth_map_texture() const {
    return depth_map->get_texture();
}

std::vector<glm::mat4> PointLight::gen_cubemap_transform() {
    std::vector<glm::mat4> shadow_transforms;
    shadow_transforms.push_back(shadow_projection * glm::lookAt(light_pos, light_pos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadow_transforms.push_back(shadow_projection * glm::lookAt(light_pos, light_pos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadow_transforms.push_back(shadow_projection * glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    shadow_transforms.push_back(shadow_projection * glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    shadow_transforms.push_back(shadow_projection * glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadow_transforms.push_back(shadow_projection * glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    return shadow_transforms;
}

void PointLight::set_light_shad_shader_vars(std::shared_ptr<Shader> shader) {
   //add this later if needed
}

void PointLight::print_glm(glm::vec3 pos) {
    std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
}

void PointLight::set_depth_map_shader() {
    std::vector<glm::mat4> shadow_transforms = gen_cubemap_transform();
    depth_shader->use();
    for (unsigned int i = 0; i < 6; ++i) {
        depth_shader->setMat4("shadowMatrices[" + std::to_string(i) + "]", shadow_transforms[i]);
    }
    depth_shader->setFloat("far_plane", far_plane);
    depth_shader->setVec3("lightPos", light_pos);
}

glm::vec3 PointLight::get_light_pos() const {
    return light_pos;
}

void PointLight::bind_shadow_map_texture() const {
    unsigned int default_text;
    glGenTextures(1, &default_text);
    glBindTexture(GL_TEXTURE_2D, default_text);

    unsigned char whitePixel[] = { 1, 1, 1, 1 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, default_text);
}

void PointLight::set_light_uniforms(std::shared_ptr<Shader> shader) {
    shader->setVec3("pointLight.position", light_pos);
    shader->setFloat("pointLight.constant", constant);
    shader->setFloat("pointLight.linear", linear);
    shader->setFloat("pointLight.quadratic", quadratic);
    shader->setVec3("pointLight.ambient", ambient);
    shader->setVec3("pointLight.diffuse", diffuse);
    shader->setVec3("pointLight.specular", specular);
}
