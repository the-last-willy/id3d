#pragma once

#include "bvh/all.hpp"
#include "grid/all.hpp"
#include "scene/all.hpp"
#include "shader/all.hpp"

#include <id3d/common/all.hpp>

void Application::init() {
    {
        shader_compiler.log_folder = "logs/";
    }
    { // Frustum culling.
        frustum_culling_shader = ::frustum_culling_shader(shader_compiler);
    }
    {
        wireframe_pass.program = agl::standard::shared(
            shader::wireframe(shader_compiler));
        bb_mesh = agl::standard::shared(
            agl::engine::instance(
                eng::Mesh()));
    }
    {
        scene = wavefront_scene("C:/Users/Willy/Desktop/data/bistro-small/exterior.obj");
        // scene = wavefront_scene("C:/Users/Willy/Desktop/data/wavefront/CornellBox/cornell-box.obj");
        // scene = wavefront_scene("D:/data/cornell-box/cornell-box.obj");
        // scene = wavefront_scene("D:/data/bistro-small/exterior.obj");

        

        scene.program = render_program(shader_compiler);
        scene.program.capabilities.emplace_back(
            agl::Capability::depth_test, 
            []() { glDepthFunc(GL_LEQUAL); });
        // { // SSBO introspection.
        //     int count = 0;
        //     glGetProgramInterfaceiv(
        //         scene.program.program,
        //         GL_SHADER_STORAGE_BLOCK,
        //         GL_ACTIVE_RESOURCES,
        //         &count);
        // }


        update_bounds(scene);
        // scene_bvh = bvh(scene);

        scene_grid = grid(scene, 32);

        {
            // scene_triangle_material_id_buffer = agl::create(agl::buffer_tag);
            // storage(
            //     scene_triangle_material_id_buffer,
            //     std::span(scene.triangle_material_ids));
            scene.triangle_material_id_ssbo
            = triangle_material_id_buffer(scene_grid, scene); 
            glBindBufferBase(
                GL_SHADER_STORAGE_BUFFER,
                0,
                scene.triangle_material_id_ssbo);

            glBindTextureUnit(0, scene.albedo_array_texture.texture);
        }

        scene.index_buffer = index_buffer(scene_grid, scene);

        initialize_gpu(scene);

        scene_grid_lights = lights(scene_grid, std::span(scene.lights));
    }
    { // 

    }
    { // Camera.
        if(auto pp = std::get_if<eng::PerspectiveProjection>(&camera.projection)) {
            pp->aspect_ratio = 16.f / 9.f;
            pp->z_far = 1000.f;
        }
    }
    { // Meshes.
        box_wireframe = agl::standard::shared(
            agl::engine::wireframe(
                gizmo::box_wireframe()));
    }
}
