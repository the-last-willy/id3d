#pragma once

#include "shader/all.hpp"

#include <agl/engine/all.hpp>

struct Tree {
    agl::Vec4 color = {1.f, 1.f, 1.f, 1.f};
    agl::Vec3 position = {0.f, 0.f, 0.f};
    float scale = 1.f;
};

struct Forest {


    // GPU program.

    agl::opengl::Program shader;

    // GPU VBOs.

    agl::opengl::Buffer index_vbo;
    agl::opengl::Buffer normal_vbo;
    agl::opengl::Buffer position_vbo;

    // GPU VAO.
    std::size_t triangle_count = 0;
    std::size_t vertex_count = 0;
    agl::opengl::VertexArray vao;

    // GPU draw parameters.
    std::size_t instance_count = 0;

    // GPU SSBOs.
    agl::opengl::Buffer color_ssbo;
    agl::opengl::Buffer model_to_world_ssbo;
};

struct ForestParameters {
    eng::ShaderCompiler* shader_compiler;
    std::vector<Tree>* trees;
};

inline
auto forest(const ForestParameters& fps) {
    auto f = Forest();
    { // Load program.
        f.shader = trees_shader(*fps.shader_compiler);
    }
    { // VBOs.
        auto triangle_count = f.triangle_count = 4;
        auto vertex_count = f.vertex_count = 5;
        {
            glNamedBufferStorage(
                f.index_vbo,
                triangle_count * sizeof(GLuint[3]),
                NULL,
                GL_MAP_WRITE_BIT);
            auto m = mapping(f.index_vbo, GL_MAP_WRITE_BIT);
            auto elements = reinterpret_cast<std::array<GLuint, 3>*>(m.mapping);
            elements[0] = {0, 1, 2};
            elements[1] = {0, 2, 3};
            elements[2] = {0, 3, 4};
            elements[3] = {0, 4, 1};
        }
        {
            glNamedBufferStorage(
                f.normal_vbo,
                vertex_count * sizeof(float[3]),
                NULL,
                GL_MAP_WRITE_BIT);
            auto m = mapping(f.normal_vbo, GL_MAP_WRITE_BIT);
            auto elements = reinterpret_cast<std::array<float, 3>*>(m.mapping);
            elements[0] = { 0.f, 1.f,  0.f};
            elements[1] = { 1.f, 0.f,  0.f};
            elements[2] = { 0.f, 0.f,  1.f};
            elements[3] = {-1.f, 0.f,  0.f};
            elements[4] = { 0.f, 0.f, -1.f};
        }
        {
            glNamedBufferStorage(
                f.position_vbo,
                3 * vertex_count * sizeof(float[3]),
                NULL,
                GL_MAP_WRITE_BIT);
            auto m = mapping(f.position_vbo, GL_MAP_WRITE_BIT);
            auto elements = reinterpret_cast<std::array<float, 3>*>(m.mapping);
            elements[0] = { 0.f,  0.f,  1.f};
            elements[1] = { 1.f,  0.f, -1.f};
            elements[2] = { 0.f,  1.f, -1.f};
            elements[3] = {-1.f,  0.f, -1.f};
            elements[4] = { 0.f, -1.f, -1.f};
        }
    }
    { // VAO.
        {
            glVertexArrayElementBuffer(f.vao, f.index_vbo);
        }
        {
            glVertexArrayAttribBinding(f.vao, 0, 0);
            glVertexArrayVertexBuffer(f.vao, 0, f.normal_vbo, 0, 12);
            glVertexArrayAttribFormat(f.vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
            glEnableVertexArrayAttrib(f.vao, 0);
        }
        {
            glVertexArrayAttribBinding(f.vao, 1, 1);
            glVertexArrayVertexBuffer(f.vao, 1, f.normal_vbo, 0, 12);
            glVertexArrayAttribFormat(f.vao, 1, 3, GL_FLOAT, GL_FALSE, 0);
            glEnableVertexArrayAttrib(f.vao, 1);
        }
    }
    { // Draw paramters.
        f.instance_count = size(*fps.trees);
    }
    if(f.instance_count > 0) { // SSBOs.
        auto& trees = *fps.trees;
        {
            glNamedBufferStorage(
                f.model_to_world_ssbo,
                size(trees) * sizeof(agl::Mat4),
                NULL,
                GL_MAP_WRITE_BIT);
            auto m = mapping(f.model_to_world_ssbo, GL_MAP_WRITE_BIT);
            auto elements = reinterpret_cast<agl::Mat4*>(m.mapping);
            for(std::size_t i = 0; i < size(trees); ++i) {
                auto& t = trees[i];
                elements[i] = agl::translation(t.position) * agl::scaling3(t.scale); 
            }
        }
        {
            glNamedBufferStorage(
                f.color_ssbo,
                size(trees) * sizeof(agl::Vec4),
                NULL,
                GL_MAP_WRITE_BIT);
            auto m = mapping(f.color_ssbo, GL_MAP_WRITE_BIT);
            auto elements = reinterpret_cast<agl::Vec4*>(m.mapping);
            for(std::size_t i = 0; i < size(trees); ++i) {
                auto& t = trees[i];
                elements[i] = t.color;
            }
        }
    }
    return f;
}


struct ForestRenderParameters {
    agl::Mat4 world_to_clip;
    agl::Mat4 world_to_view;
};

inline
void render(Forest& f, const ForestRenderParameters& frps) {
    if(f.instance_count == 0) {
        return;
    }
    {
        glUseProgram(f.shader);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }
    {
        glBindVertexArray(f.vao);
    }
    {
        glUniformMatrix4fv(0, 1, GL_FALSE, element_data(frps.world_to_clip));
        glUniformMatrix4fv(agl::attribute_location(agl::Program(f.shader.name), "world_to_view"), 1, GL_FALSE, element_data(frps.world_to_view));
    }
    {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, f.color_ssbo);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, f.model_to_world_ssbo);
    }
    {
        glDrawElementsInstancedBaseVertexBaseInstance(
            GL_TRIANGLES,
            GLsizei(3 * f.triangle_count),
            GL_UNSIGNED_INT,
            0,
            GLsizei(f.instance_count),
            0,
            0);
    }
    {
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
    }
}
