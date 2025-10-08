#include "depth_map_2d.hpp"

DepthMap2D::DepthMap2D()
    : SHADOW_WIDTH(1024), SHADOW_HEIGHT(1024), depthMapFBO(0), depth_map_2d(0)
{
   
}

void DepthMap2D::init(unsigned int shadow_width, unsigned int shadow_height)
{
    SHADOW_WIDTH = shadow_width;
    SHADOW_HEIGHT = shadow_height;

   
    glGenFramebuffers(1, &depthMapFBO);

   
    glGenTextures(1, &depth_map_2d);

    gen_cube_map();         
    bind_depth_to_frame_buf_return(); 
}

unsigned int DepthMap2D::get_fbo() const
{
    return depthMapFBO;
}

unsigned int DepthMap2D::get_texture() const
{
    return depth_map_2d;
}

void DepthMap2D::gen_cube_map()
{
    glBindTexture(GL_TEXTURE_2D, depth_map_2d);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

void DepthMap2D::bind_depth_to_frame_buf_return()
{
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map_2d, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

DepthMap2D::~DepthMap2D()
{
   
}
