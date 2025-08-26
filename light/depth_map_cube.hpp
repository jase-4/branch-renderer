#ifndef DEPTH_MAP_CUBE_HPP
#define DEPTH_MAP_CUBE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "depth_map.hpp"

class DepthMapCubeMap : public DepthMap
{
private:
    unsigned int SHADOW_WIDTH;
    unsigned int SHADOW_HEIGHT;
    unsigned int depthMapFBO;
    unsigned int depthCubemap;

public:
    DepthMapCubeMap();
    ~DepthMapCubeMap();

    void init(unsigned int shadow_width, unsigned int shadow_height) override;
    unsigned int get_fbo() const override;
    unsigned int get_texture() const override;

    void gen_cube_map();
    void bind_depth_to_frame_buf_return();
};

#endif // DEPTH_MAPCUBE_H
