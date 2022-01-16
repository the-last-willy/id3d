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
            if(auto pp = std::get_if<eng::PerspectiveProjection>(&frustum_culling_camera.projection)) {
                pp->z_far = 100.f;
            }
        }
    }
    { // Occlusion culling.
        if(not settings.occlusion_culling.is_anchored) {
            occlusion_culling_camera = camera;
        }
    }
    // Ray tracer.
    if(settings.ray_tracer.is_enabled) {
        if(settings.ray_tracer.is_shooting) {
            auto c2w = agl::engine::clip_to_world(camera);
            auto rd = std::uniform_real_distribution<float>(-1.f, 1.f);
            for(int i = 0; i < 1'000; ++i) {
                auto r = Ray{
                    .origin = camera.view.position,
                    .direction = agl::normalize((c2w * agl::vec4(
                        0.2f * rd(random),
                        0.2f * rd(random),
                        1.f,
                        1.f)
                    ).xyz())};
                trace(ray_tracer, scene, r);
            }
            statistics.ray_tracer.point_count = ray_tracer.pc.size;
        }
    }
}
