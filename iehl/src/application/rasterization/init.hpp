#pragma once

#include "bvh/all.hpp"
#include "grid/all.hpp"
#include "scene/all.hpp"
#include "shader/all.hpp"

#include <id3d/common/all.hpp>

void Application::init() {
    { // Shader compiler.
        shader_compiler.log_folder = "logs/";
    }
    { // Forward rendering.
        forward_renderer = ::forward_renderer(shader_compiler);
    }
    // {
    //     wireframe_pass.program = agl::standard::shared(
    //         shader::wireframe(shader_compiler));
    //     bb_mesh = agl::standard::shared(
    //         agl::engine::instance(
    //             eng::Mesh()));
    // }
    { // Scene.
        // scene = wavefront_scene("C:/Users/Willy/Desktop/data/bistro-small/exterior.obj");
        // scene = wavefront_scene("C:/Users/Willy/Desktop/data/wavefront/CornellBox/cornell-box.obj");
        // scene = wavefront_scene("D:/data/cornell-box/cornell-box.obj");
        scene = wavefront_scene("D:/data/bistro-small/exterior.obj");

        // { // SSBO introspection.
        //     int count = 0;
        //     glGetProgramInterfaceiv(
        //         scene.program.program,
        //         GL_SHADER_STORAGE_BLOCK,
        //         GL_ACTIVE_RESOURCES,
        //         &count);
        // }


        update_bounds(scene);
        // // scene_bvh = bvh(scene);

        // scene_grid = grid(scene, 32);

        // {
        //     // scene_triangle_material_id_buffer = agl::create(agl::buffer_tag);
        //     // storage(
        //     //     scene_triangle_material_id_buffer,
        //     //     std::span(scene.triangle_material_ids));
        //     scene.vertex_attribute_group.triangle_material_id_ssbo
        //     = triangle_material_id_buffer(scene_grid, scene); 
        //     glBindBufferBase(
        //         GL_SHADER_STORAGE_BUFFER,
        //         0,
        //         scene.vertex_attribute_group.triangle_material_id_ssbo);

        //     glBindTextureUnit(0, scene.material_group.albedo_texture_array);
        // }

        // scene.object_group.element_buffer = index_buffer(scene_grid, scene);

        // initialize_gpu(scene);

        // light_grid = ::light_grid(
        //     scene.lights,
        //     scene.bounds,
        //     {10, 10, 10});
        // scene_grid_lights = lights(scene_grid, std::span(scene.lights));
    }
    { // Camera.
        if(auto pp = std::get_if<eng::PerspectiveProjection>(&camera.projection)) {
            pp->aspect_ratio = 16.f / 9.f;
            pp->z_far = 1000.f;
        }
        frustum_culling_camera = camera;
    }
    // { // Meshes.
    //     box_wireframe = agl::standard::shared(
    //         agl::engine::wireframe(
    //             gizmo::box_wireframe()));
    // }
    
    { // Forward rendering vao.
        forward_rendering_vao = vertex_array(
            forward_renderer, scene.vertex_attribute_group);
    }
    { // Light culling.
        std::cout << "Light culling." << std::endl;
        light_culling = ::light_culling(scene.light_group, scene.bounds, {32, 32, 32});
    }
    std::cout << "Initialization done." << std::endl;
}
