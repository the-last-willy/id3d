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
    oc.depth_images.emplace_back(pixels);
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

        w = w2;
        h = h2;
    }
}
