void Application::update(float) {
    if(not ImGui::GetIO().WantCaptureMouse) {
        if(ImGui::IsMouseDown(0)) {
            glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            auto d = ImGui::GetIO().MouseDelta;
            camera.view.yaw -= float(d[0]) / 500.f;
            camera.view.pitch -= float(d[1]) / 500.f;
        } else {
            glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    // Camera controls.
    if(not ImGui::GetIO().WantCaptureKeyboard) { 
        if(glfwGetKey(window.window, GLFW_KEY_A)) {
            auto direction = (rotation(camera.view)
            * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
            camera.view.position = camera.view.position - direction / 3.f;
        }
        if(glfwGetKey(window.window, GLFW_KEY_D)) {
            auto direction = (rotation(camera.view)
            * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
            camera.view.position = camera.view.position + direction / 3.f;
        }
        if(glfwGetKey(window.window, GLFW_KEY_S)) {
            auto direction = rotation(camera.view)[2].xyz();
            camera.view.position = camera.view.position + direction / 3.f;
        }
        if(glfwGetKey(window.window, GLFW_KEY_W)) {
            auto direction = rotation(camera.view)[2].xyz();
            camera.view.position = camera.view.position - direction / 3.f;
        }
    }
    { // Frustum culling.
        if(not settings.frustum_culling.is_anchored) {
            frustum_culling_camera = camera;
        }
    }
    { // Occlusion culling.
        if(not settings.occlusion_culling.is_anchored) {
            occlusion_culling_camera = camera;
        }
    }
}
