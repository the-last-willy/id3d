#pragma once

#include "bvh/all.hpp"
#include "grid/all.hpp"
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
        
        // grid_subdivision(scene.objects, {4, 4, 4});
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
    
    std::cout << "Initialization done." << std::endl;
}
