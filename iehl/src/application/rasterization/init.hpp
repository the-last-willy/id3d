#pragma once

#include "bvh/all.hpp"
#include "scene/all.hpp"

#include <id3d/common/all.hpp>

void Application::init() {
    { // Shader compiler.
        shader_compiler.log_folder = "logs/";
    }
    { // Forward rendering.
        
    }
    { // Scene.
        // scene = wavefront_scene("C:/Users/Willy/Desktop/data/bistro-small/exterior.obj");
        // scene = wavefront_scene("C:/Users/Willy/Desktop/data/wavefront/CornellBox/cornell-box.obj");
        scene = wavefront_scene("D:/data/cornell-box/cornell-box.obj");
        // scene = wavefront_scene("D:/data/bistro-small/exterior.obj");
        
        grid_subdivision(scene.objects, {10, 10, 10});
    }
    { // Camera.
        if(auto pp = std::get_if<eng::PerspectiveProjection>(&camera.projection)) {
            pp->aspect_ratio = 16.f / 9.f;
            pp->z_far = 1000.f;
        }
        frustum_culling_camera = camera;
    }

    { // Forward rendering.
        forward_renderer = ::forward_renderer(shader_compiler);
        forward_rendering_vao = vertex_array(
            forward_renderer, scene.objects.vertex_attributes);
    }
    { // Occlusion culling.
        occlusion_culler = ::occlusion_culler(shader_compiler);
    }
    { // Tone mapping.
        tone_mapper = ::tone_mapper(shader_compiler);
    }
    { // Light culling.
        std::cout << "Light culling." << std::endl;
        light_culling = ::light_culling(scene.light_group, scene.bounds, {16, 16, 16});
    }

    { // Gizmos.
        wireframe_renderer = ::wireframe_renderer(shader_compiler);
        wire_box_vao = vertex_array(
            wire_box.vertex_attributes, wireframe_renderer);
        wire_quad_vao = vertex_array(
            wire_quad.vertex_attributes, wireframe_renderer);
    }
    
    std::cout << "Initialization done." << std::endl;
}
