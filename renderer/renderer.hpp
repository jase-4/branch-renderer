#ifndef RENDERER_HPP
#define RENDERER_HPP


#include <stb_image.h>

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <string>

#include "scene/mesh_object.hpp"
#include "core/globals.hpp"
#include "light/dir_light.hpp"
#include "light/spotlight.hpp"
#include "light/point_light.hpp"
#include "render_components.hpp"
#include "entity/entity_manager.hpp"

extern unsigned int quadVAO;
extern unsigned int quadVBO;

void renderQuad();
unsigned int loadTexture(std::string path);

class Renderer{
private:
  
    std::vector<std::unique_ptr<Light>> lights;
  

public:
    Renderer();
    ~Renderer();
    void render();
    void init();
    void gen_shadow_map();

    std::shared_ptr<Shader> light_shad_shader;
    unsigned int woodTexture ;
    unsigned int depthMapFBO ;
    unsigned int depthCubemap ;
    unsigned int default_text ;
    EntityManager& manager = EntityManager::getInstance();
    void draw_models(Shader &shader, std::vector<MeshComponent> meshes,bool draw_textures);
    void render_objects(Shader &shader, bool set_textures);
    void gen_shadow_maps();
    void set_lights_uniforms( std::shared_ptr<Shader> shader);
    void bind_shadow_map_textures();

 
};

#endif