#pragma once

#include "application.hpp"

void update_terrain(App& a) {
    if(a.settings.is_slope_outdated) {
        a.settings.is_slope_outdated = false;

        a.settings.is_drainage_area_outdated = true;
        a.settings.is_laplacian_outdated = true;
        a.settings.is_mesh_outdated = true;
        a.settings.is_normal_outdated = true;
    }

    if(a.settings.is_drainage_area_outdated) {
        a.settings.is_drainage_area_outdated = false;

        if(a.settings.drainage_area_formula == DrainageAreaFormula::steepest) {
            steepest_compute(a.terrain);
        } else if(a.settings.drainage_area_formula == DrainageAreaFormula::two_steepest) {
             
        } else if(a.settings.drainage_area_formula == DrainageAreaFormula::weighted) {
            mean_compute(a.terrain);
        }

        a.settings.is_color_outdated = true;
    }

    if(a.settings.is_laplacian_outdated) {
        a.settings.is_laplacian_outdated = false;

        update_laplacian(a.terrain);
        
        a.settings.is_color_outdated = true;
    }

    if(a.settings.is_normal_outdated) {
        a.settings.is_normal_outdated = false;

        update_normals_using_gradient(a.terrain);

        a.settings.is_color_outdated = true;
    }

    if(a.settings.is_color_outdated) {
        a.settings.is_color_outdated = false;

        if(a.settings.color_formula == ColorFormula::drainage_area) {
            update_colors_using_drainage_area(a.terrain);
        } else if(a.settings.color_formula == ColorFormula::laplacian) {
            update_colors_using_laplacian(a.terrain);
        } else if(a.settings.color_formula == ColorFormula::normals) {
            update_colors_using_normals(a.terrain);
        } else if(a.settings.color_formula == ColorFormula::slope) {
            update_colors_using_slope(a.terrain);
        }

        a.settings.is_mesh_outdated = true;
    }

    if(a.settings.is_mesh_outdated) {
        a.settings.is_mesh_outdated = false;

        update_cpu(a.terrain);
        update_gpu(a.terrain);
    }
}

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
    update_terrain(*this);
}
