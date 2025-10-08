#include "spotlight.hpp"
#include "depth_map_2d.hpp"

Spotlight::Spotlight() {
}

Spotlight::~Spotlight() {
}

void Spotlight::init_depth_map() {
    std::cout << shadow_width << std::endl;

    std::unique_ptr<DepthMap2D> depth_map_2d = std::make_unique<DepthMap2D>();
    depth_map = std::move(depth_map_2d);
    depth_map->init(shadow_width, shadow_height);
}

std::shared_ptr<Shader> Spotlight::get_depth_shader() const {
    return depth_shader;
}

void Spotlight::set_light_pos(glm::vec3 new_pos) {
    light_pos = new_pos;
}

unsigned int Spotlight::get_shadow_width() const {
    return shadow_width;
}

unsigned int Spotlight::get_shadow_height() const {
    return shadow_height;
}

float Spotlight::get_far_plane() const {
    return far_plane;
}

float Spotlight::get_near_plane() const {
    return near_plane;
}

GLuint Spotlight::get_depth_map_FBO() const {
    return depth_map->get_fbo();
}

unsigned int Spotlight::get_depth_map_texture() const {
    return depth_map->get_texture();
}

void Spotlight::set_light_shad_shader_vars(std::shared_ptr<Shader> shader) {
    glm::mat4 lightView = glm::lookAt(light_pos, targetPos, upVector);
    glm::mat4 lightSpaceMatrix = light_projection * lightView;
    shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
}

void Spotlight::set_depth_map_shader() {
    glm::mat4 lightView = glm::lookAt(light_pos, targetPos, upVector);
    glm::mat4 lightSpaceMatrix = light_projection * lightView;

    depth_shader->use();
    depth_shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
}

glm::vec3 Spotlight::get_light_pos() const {
    return light_pos;
}

void Spotlight::bind_shadow_map_texture() const {
    unsigned int default_text;
    glGenTextures(1, &default_text);
    glBindTexture(GL_TEXTURE_2D, default_text);

    unsigned char whitePixel[] = { 1, 1, 1, 1 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, default_text);
}

void Spotlight::set_light_uniforms(std::shared_ptr<Shader> shader) {
    direction = glm::normalize(targetPos - light_pos);

    shader->setVec3("spotLight.position", light_pos);
    shader->setVec3("spotLight.direction", direction);
    shader->setFloat("spotLight.cutOff", cut_off);
    shader->setFloat("spotLight.outerCutOff", outer_cut_off);
    shader->setFloat("spotLight.constant", constant);
    shader->setFloat("spotLight.linear", linear);
    shader->setFloat("spotLight.quadratic", quadratic);
    shader->setVec3("spotLight.ambient", ambient);
    shader->setVec3("spotLight.diffuse", diffuse);
    shader->setVec3("spotLight.specular", specular);
}
