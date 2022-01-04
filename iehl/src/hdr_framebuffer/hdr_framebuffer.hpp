#pragma once

#include "opengl.hpp"

struct HDR_Framebuffer {
    gl::Framebuffer fbo;

    gl::Texture color_texture = GL_TEXTURE_2D;
    gl::Texture depth_texture = GL_TEXTURE_2D;

    HDR_Framebuffer() {
        // Color texture.

    	glTextureStorage2D(color_texture,
            1, GL_R11F_G11F_B10F, 1280, 720);

        // Depth texture.

        glTextureStorage2D(depth_texture,
            1, GL_DEPTH_COMPONENT32F, 1280, 720);

        // Framebuffer.

        glNamedFramebufferTexture(fbo,
            GL_COLOR_ATTACHMENT0,
            color_texture, 0);
        
        glNamedFramebufferTexture(fbo,
            GL_DEPTH_ATTACHMENT,
            depth_texture, 0);
    }
};
