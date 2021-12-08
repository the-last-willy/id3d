#pragma once

#include "opengl/draw_elements_parameters.hpp"
#include "scene.hpp"

#include <agl/engine/all.hpp>

#include <span>

inline
void render(Scene& s) {
    bind(s.program);
    bind(s.vertex_array);

    agl::draw_elements(
        agl::DrawMode::triangles,
        agl::Count<GLsizei>(GLsizei(3 * size(s.triangle_indices))),
        agl::DrawType::unsigned_int,
        agl::Offset<GLintptr>(0));

    // auto count = GLsizei(3 * size(s.triangle_indices));
    // auto offset = GLintptr(0);
    // glMultiDrawElements(
    //     GLenum(agl::DrawMode::triangles),
    //     &count,
    //     GLenum(agl::DrawType::unsigned_int),
    //     reinterpret_cast<const void * const *>(&offset),
    //     1);

    unbind(s.program);
}

inline
void render(Scene& s, std::span<GLsizei> counts, std::span<GLintptr> offsets) {
    bind(s.program);
    bind(s.vertex_array);

    { // SSBOs.
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, s.material_ssbo);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, s.light_ssbo);
    }
    { // Uniforms.
        eng::uniform(s.program, "light_count", int(size(s.lights)));
    }

    glMultiDrawElements(
        GLenum(agl::DrawMode::triangles),
        data(counts),
        GLenum(agl::DrawType::unsigned_int),
        reinterpret_cast<const void * const *>(data(offsets)),
        GLsizei(size(counts)));
    
    unbind(s.program);
}

inline
void render(Scene& s, std::span<DrawElementsParameters> deps) {
    bind(s.program);
    bind(s.vertex_array);

    { // SSBOs.
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, s.material_ssbo);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, s.light_ssbo);
    }
    { // Uniforms.
        eng::uniform(s.program, "light_count", int(size(s.lights)));
    }

    auto indirect = agl::create(agl::buffer_tag);
    storage(indirect, deps);

    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect);
    glMultiDrawElementsIndirect(
        GL_TRIANGLES,
        GL_UNSIGNED_INT,
        0,
        GLsizei(size(deps)),
        0);

    delete_(indirect);
    
    unbind(s.program);
}
