#include "renderer.hpp"

unsigned int quadVAO = 0;
unsigned int quadVBO = 0;


void Renderer::init(){
    std::unique_ptr<Spotlight> dir_light = std::make_unique<Spotlight>(); 
    dir_light->init_depth_map();
    std::unique_ptr<PointLight> point = std::make_unique<PointLight>(); 
    point->init_depth_map();

    std::unique_ptr<Light> light = std::move(dir_light);
    std::unique_ptr<Light> light2 = std::move(point);
    lights.emplace_back(std::move(light));
    lights.emplace_back(std::move(light2));
    light_shad_shader = ShaderManager::getInstance().get_shader("lighting_shadow");
    light_shad_shader->use();
    light_shad_shader->setInt("material.diffuse", 0);
    light_shad_shader->setInt("material.specular", 1);
    light_shad_shader->setInt("dirDepthMap", 2);
    light_shad_shader->setInt("depthCubeMap", 3);

    glGenTextures(1, &default_text);
    glBindTexture(GL_TEXTURE_2D,default_text);
    unsigned char whitePixel[] = { 255, 255, 255, 0 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void Renderer::render(){

    float time = glfwGetTime();  
    float minValue = -20.0f;         
    float maxValue = 20.0f;
    float minValue1 = 2.0f;        
    float maxValue1 = 30.0f;
    float range = maxValue - minValue; 
    float range1 = maxValue1 - minValue1; 
    float speed = 0.5f;      
    float speed1 = 1.0f;      
    float light_pos_z = sin(time * speed) * range / 2.0f + (minValue + maxValue) / 2.0f;
    float light_pos_y = sin(time * speed1) * range1 / 2.0f + (minValue1 + maxValue1) / 2.0f;
    //light_pos_z = 10.0f;
    lights[0]->set_light_pos(glm::vec3(lights[0]->get_light_pos().x,light_pos_y,light_pos_z));


    gen_shadow_maps();
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    light_shad_shader->use();
    set_lights_uniforms(light_shad_shader);
    light_shad_shader->setFloat("material.shininess", 32.0f);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    light_shad_shader->setMat4("projection", projection);
    light_shad_shader->setMat4("view", view);
    light_shad_shader->setVec3("viewPos", camera.Position);

  
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,default_text);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,lights[0]->get_depth_map_texture());
    glActiveTexture(GL_TEXTURE3);
     glBindTexture(GL_TEXTURE_CUBE_MAP, lights[1]->get_depth_map_texture());

     lights[0]->set_light_shad_shader_vars(light_shad_shader);

    render_objects(*light_shad_shader,true);

    // lights[1]->set_light_shad_shader_vars(light_shad_shader);
    //render_objects(*light_shad_shader,true);

    // shader2.use();
    // shader2.setMat4("projection", projection);
    // shader2.setMat4("view", view);
    // light_cube.draw(shader2);
    // std::shared_ptr<Shader>  depth_quad = ShaderManager::getInstance().get_shader("depth_quad");
    // depth_quad->use();
    // depth_quad->setFloat("near_plane",lights[0]->get_near_plane());
    // depth_quad->setFloat("far_plane",lights[0]->get_far_plane());
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, lights[0]->get_depth_map_texture());
    //renderQuad();

}



    void Renderer::draw_models(Shader &shader, std::vector<MeshComponent> meshes,bool draw_textures) 
    {
    for(int i = 0; i < meshes.size(); i++ ){

            if (draw_textures = true){
                for(unsigned int j = 0; j < meshes[i].textures.size(); j++)
                {
                    
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D,meshes[i].textures[j].id);
             
                }
            }
          
                glBindVertexArray(meshes[i].VAO);
                glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(meshes[i].indices.size()), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);

          
            glActiveTexture(GL_TEXTURE0);
            }
        }

    void Renderer::render_objects(Shader &shader, bool set_textures){
     std::vector<Entity::ID> entity_list = manager.getEntitiesWithComponents<RenderableComponent,TransformComponent>();
        for(int i = 0; i < entity_list.size(); i++){       
           auto render_component = manager.getComponent<RenderableComponent>(entity_list[i]);
            if(!render_component->should_render){
                continue;
            }
           auto transform = manager.getComponent<TransformComponent>(entity_list[i]);
           glm::mat4 model_matrix = transform->getModelMatrix();
           shader.setMat4("model", model_matrix);
           draw_models(shader,render_component->meshes,set_textures);
        }
    }

    void Renderer::gen_shadow_maps(){
        glEnable(GL_DEPTH_TEST);
        for (auto& light : lights) {
            glViewport(0, 0, light->get_shadow_width(), light->get_shadow_height());
            glBindFramebuffer(GL_FRAMEBUFFER, light->get_depth_map_FBO());
            glClear(GL_DEPTH_BUFFER_BIT);
            light->set_depth_map_shader();
            render_objects(*(light->get_depth_shader()),false);
         
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
       
    }

    void Renderer::set_lights_uniforms( std::shared_ptr<Shader> shader){
    
        for(auto& light : lights){
            light->set_light_uniforms(shader);
          
        }
    }

    void Renderer::bind_shadow_map_textures(){
        for(auto& light : lights){
            light->bind_shadow_map_texture();
        }
    }


Renderer::Renderer(/* args */)
{ 
}

Renderer::~Renderer()
{
}

unsigned int loadTexture(std::string path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}


