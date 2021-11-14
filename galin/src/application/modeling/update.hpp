#pragma once

#include "application.hpp"


void App::update(float) {
    if(not ImGui::GetIO().WantCaptureMouse) {
        if(glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_1)) {
            glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            agl::Vec2 d = current_cursor_pos - previous_cursor_pos;
            camera.view.yaw -= d[0] / 500.f;
            camera.view.pitch -= d[1] / 500.f;

            previous_cursor_pos = current_cursor_pos;
        } else {
            glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            previous_cursor_pos = current_cursor_pos;
        }
    }
    {
        if(glfwGetKey(window.window, GLFW_KEY_A)) {
            auto direction = (rotation(camera.view) * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
            camera.view.position = camera.view.position - direction / 10.f;
        }
        if(glfwGetKey(window.window, GLFW_KEY_D)) {
            auto direction = (rotation(camera.view) * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
            camera.view.position = camera.view.position + direction / 10.f;
        }
        if(glfwGetKey(window.window, GLFW_KEY_S)) {
            auto direction = rotation(camera.view)[2].xyz();
            camera.view.position = camera.view.position + direction / 10.f;
        }
        if(glfwGetKey(window.window, GLFW_KEY_W)) {
            auto direction = rotation(camera.view)[2].xyz();
            camera.view.position = camera.view.position - direction / 10.f;
        }
    }
}

void App::update_object() {
    { // Load patches.
        auto cbm = load_teapot();
        object.patches.resize(size(cbm.patches));
        for(std::size_t h = 0; h < size(cbm.patches); ++h) {
            auto& p = cbm.patches[h];
            auto g = agl::common::Grid<agl::Vec3>(
                agl::common::grid_indexing({4, 4}));
            for(uint32_t i = 0; i < 4; ++i)
            for(uint32_t j = 0; j < 4; ++j) {
                at(g, i, j) = cbm.vertices[p[4 * i + j]];
            }
            auto& pa = object.patches[h];
            pa.control_points = std::move(g);
            update_control_mesh(pa);
        }
    }
    { // Tesselation.
        tessellate(object, std::size_t(settings.tesselation_resolution));
    }
    { // Transform.
        if(settings.transform_enabled) {
            auto attenuation = attenuation_function(
                TransformAttenuation(settings.transform_attenuation));
            if(settings.transform_type == int(TransformType::attract)) {
                auto transformation = std::function<agl::Vec3(agl::Vec3)>();
                transform(object, [&](agl::Vec3 p) {
                    auto d = settings.transform_position - p;
                    auto a = attenuation((settings.transform_position - p) / settings.transform_radius);
                    return p + a * d * settings.transform_intensity;
                });
            } else if(settings.transform_type == int(TransformType::extrude)) {
                auto transformation = std::function<agl::Vec3(agl::Vec3)>();
                transform(object, [&](agl::Vec3 p) {
                    auto a = attenuation((settings.transform_position - p) / settings.transform_radius);
                    return p + a * settings.transform_intensity * agl::vec3(0.f, 1.f, 0.f);
                });
            } else if(settings.transform_type == int(TransformType::twist)) {
                auto transformation = std::function<agl::Vec3(agl::Vec3)>();
                transform(object, [&](agl::Vec3 p) {
                    auto o = settings.transform_position;
                    auto v = p - o;
                    auto a = attenuation(v / ssd);
                    auto angle = length(v) * a * settings.transform_intensity;
                    return (agl::rotation_y(angle) * agl::vec4(v, 0.f)).xyz() + o;
                });
            }
        }
    }
    { // FFD.
        if(settings.ffd_enabled) {
            update_bounding_box(object);
            auto norm_a = 1.f / length(object.bounding_box);
            auto norm_b = -lower_bound(object.bounding_box) * norm_a;
            transform(object, [&](agl::Vec3 p) {
                auto uvw = norm_a * p + norm_b;
                return bezier(ffd->control_points, uvw[0], uvw[1], uvw[2]);
            });
        }
    }
    { // GPU.
        update_bounding_box(object);
        update_gpu(object);
    }
}
