// #pragma once

// #include "opengl/draw_elements_parameters.hpp"
// #include "scene.hpp"

// #include <agl/engine/all.hpp>

// #include <span>

// inline
// void render(Scene& s, std::span<DrawElementsParameters> deps) {
//     // bind(s.program);
//     // glBindVertexArray(s.object_group.vertex_array);

//     { // SSBOs.
//         // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, s.material_ssbo);
//         // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, s.light_ssbo);
//     }
//     { // Uniforms.
//         // eng::uniform(s.program, "light_count", int(size(s.lights.lights)));
//     }

//     auto indirect = agl::create(agl::buffer_tag);
//     storage(indirect, deps);

//     glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect);
//     glMultiDrawElementsIndirect(
//         GL_TRIANGLES,
//         GL_UNSIGNED_INT,
//         0,
//         GLsizei(size(deps)),
//         0);

//     delete_(indirect);
    
//     // unbind(s.program);
// }
