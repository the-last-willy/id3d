
void GltfProgram::update(float dt) {
    { // Increment time.
        time += dt;
    }
    if(not ImGui::GetIO().WantCaptureMouse) {
        if(glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_1)) {
            glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            agl::Vec2 d = current_cursor_pos - previous_cursor_pos;
            camera->view.yaw -= d[0] / 500.f;
            camera->view.pitch -= d[1] / 500.f;

            previous_cursor_pos = current_cursor_pos;
        } else {
            glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            previous_cursor_pos = current_cursor_pos;
        }
    }
    { // Camera controls.
        if(glfwGetKey(window.window, GLFW_KEY_A)) {
            auto direction = (rotation(camera->view)
            * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
            camera->view.position = camera->view.position - direction / 10.f;
        }
        if(glfwGetKey(window.window, GLFW_KEY_D)) {
            auto direction = (rotation(camera->view)
            * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
            camera->view.position = camera->view.position + direction / 10.f;
        }
        if(glfwGetKey(window.window, GLFW_KEY_S)) {
            auto direction = rotation(camera->view)[2].xyz();
            camera->view.position = camera->view.position + direction / 10.f;
        }
        if(glfwGetKey(window.window, GLFW_KEY_W)) {
            auto direction = rotation(camera->view)[2].xyz();
            camera->view.position = camera->view.position - direction / 10.f;
        }
        if(glfwGetKey(window.window, GLFW_KEY_R)) {
            reload_shaders();
        }
    }

    if(glfwGetKey(window.window, GLFW_KEY_SPACE)) {
        toggle_rasterization = not toggle_rasterization;
    }
    if(ray_tracing_settings.ray_per_frame > 0){
        rtx(); // on
        reload_points();
    }
    if(not settings.debugging.is_frustrum_anchored) {
        *frustrum_camera = *camera;
    }
    update_render_passes();
}
