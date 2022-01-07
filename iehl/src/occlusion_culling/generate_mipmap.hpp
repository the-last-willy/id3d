#pragma once

#include "occlusion_culler.hpp"

#include <vector>

inline
void generate_mipmap(OcclusionCuller& oc) {
    auto w = oc.width;
    auto h = oc.height;
    auto pixels = std::vector<GLfloat>(w * h);
    glGetTextureImage(oc.depth_texture, 0,
        GL_DEPTH_COMPONENT, GL_FLOAT,
        GLsizei(size(pixels) * sizeof(GLfloat)), data(pixels));
    oc.depth_images.clear();
    oc.depth_image_widths.clear();
    oc.depth_image_heights.clear();
    oc.depth_images.emplace_back(pixels);
    oc.depth_image_widths.emplace_back(w);
    oc.depth_image_heights.emplace_back(h);
    for(int i = 1; i < 8; ++i) {
        auto w2 = w / 2;
        auto h2 = h / 2;

        for(int y = 0; y < h2; ++y)
        for(int x = 0; x < w2; ++x) {
            auto p00 = pixels[(2 * x + 0) + (2 * y + 0) * w];
            auto p01 = pixels[(2 * x + 0) + (2 * y + 1) * w];
            auto p10 = pixels[(2 * x + 1) + (2 * y + 0) * w];
            auto p11 = pixels[(2 * x + 1) + (2 * y + 1) * w];

            pixels[x + y * w2] = std::max(
                std::max(p00, p01),
                std::max(p10, p11));
        }

        glTextureSubImage2D(oc.depth_texture, i,
            0, 0,
            w2, h2,
            GL_DEPTH_COMPONENT, GL_FLOAT,
            data(pixels));

        pixels.resize(w2 * h2);
        oc.depth_images.emplace_back(pixels);
        oc.depth_image_widths.emplace_back(w2);
        oc.depth_image_heights.emplace_back(h2);

        w = w2;
        h = h2;
    }
}

inline
void generate_mipmap2(OcclusionCuller& oc) {
    constexpr auto input_binding = GLint(0);
    constexpr auto output_binding = GLint(1);

    glUseProgram(oc.mipmap_program);

    auto w = oc.width;
    auto h = oc.height;

    oc.depth_images.clear();
    oc.depth_image_widths.clear();
    oc.depth_image_heights.clear();

    auto pixels = std::vector<GLfloat>(w * h);
    glGetTextureImage(oc.depth_texture, 0,
        GL_DEPTH_COMPONENT, GL_FLOAT,
        GLsizei(size(pixels) * sizeof(GLfloat)), data(pixels));

    oc.depth_images.emplace_back(pixels);
    oc.depth_image_widths.emplace_back(w);
    oc.depth_image_heights.emplace_back(h);

    for(int i = 0; i < 7; ++i) {
        w /= 2;
        h /= 2;
        glBindImageTexture(input_binding,
            oc.depth_texture, i,
            GL_FALSE, 0,
            GL_READ_ONLY,
            GL_R32F);
        glProgramUniform1i(oc.mipmap_program,
            oc.mipmap_input_image_uniform_location,
            input_binding);
        glBindImageTexture(output_binding,
            oc.depth_texture, i + 1,
            GL_FALSE, 0,
            GL_WRITE_ONLY,
            GL_R32F);
        glProgramUniform1i(oc.mipmap_program,
            oc.mipmap_input_image_uniform_location,
            output_binding);
        glDispatchCompute(w / 16, h / 16, 0);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);

        pixels.resize(w * h);

        glGetTextureImage(oc.depth_texture, i + 1,
            GL_DEPTH_COMPONENT, GL_FLOAT,
            GLsizei(size(pixels)) * sizeof(GLfloat), data(pixels));

        oc.depth_images.emplace_back(pixels);
        oc.depth_image_widths.emplace_back(w);
        oc.depth_image_heights.emplace_back(h);
    }
}
