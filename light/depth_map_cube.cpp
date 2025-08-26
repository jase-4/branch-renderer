#include "depth_map_cube.hpp"

DepthMapCubeMap::DepthMapCubeMap()
    : SHADOW_WIDTH(1024), SHADOW_HEIGHT(1024), depthMapFBO(0), depthCubemap(0)
{
   
}

void DepthMapCubeMap::init(unsigned int shadow_width, unsigned int shadow_height)
{
    SHADOW_WIDTH = shadow_width;
    SHADOW_HEIGHT = shadow_height;
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthCubemap);
    gen_cube_map();              
    bind_depth_to_frame_buf_return(); 
}

unsigned int DepthMapCubeMap::get_fbo() const
{
    return depthMapFBO;
}

unsigned int DepthMapCubeMap::get_texture() const
{
    return depthCubemap;
}

void DepthMapCubeMap::gen_cube_map()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void DepthMapCubeMap::bind_depth_to_frame_buf_return()
{
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

DepthMapCubeMap::~DepthMapCubeMap()
{
  
}
