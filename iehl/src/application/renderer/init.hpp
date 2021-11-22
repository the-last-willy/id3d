void GltfProgram::init() {
    { // Shader compiler.
        shader_compiler.log_folder = "logs/";
        shader_compiler.root = "iehl/src/shader";
    }
    { // Read objects from file.
        auto content = agl::format::wavefront::load(
            "D:/data/cornell-box/cornell-box.obj",
            "D:/data/cornell-box");
            // "C:/Users/Willy/Desktop/data/wavefront/CornellBox/cornell-box.obj",
            // "C:/Users/Willy/Desktop/data/wavefront/CornellBox");
            // "D:/data/bistro-small/exterior.obj",
            // "D:/data/bistro-small/");
            // "C:/Users/Willy/Desktop/data/bistro-small/exterior.obj",
            // "C:/Users/Willy/Desktop/data/bistro-small/");
            // "C:/Users/yoanp/Documents/bistro-small/exterior.obj",
            // "C:/Users/yoanp/Documents/bistro-small/");
        for(std::size_t i = 0; i < size(content.meshes); ++i) {
            auto& o = rasterization_scene.objects.emplace_back();
            o.cpu_mesh = content.tmeshes[i];
            o.gpu_mesh = content.meshes[i];
            o.bounding_box = bounding_box(*o.cpu_mesh);
        }
        rasterization_scene.materials = std::move(content.materials);
    }

    { // Normalize data.
        auto default_albedo = std::make_shared<eng::Texture>(
            data::uniform_texture(agl::vec3(1.f)));
        auto default_emissive = std::make_shared<eng::Texture>(
            data::uniform_texture(agl::vec3(0.f)));
        auto default_specular = std::make_shared<eng::Texture>(
            data::uniform_texture(agl::vec3(0.f, 1.f, 0.f)));
        for(auto& o : rasterization_scene.objects) {
            for(auto&& p : o.gpu_mesh->primitives | ranges::views::indirect) {
                if(p.material) {
                    auto&& ma = *p.material;
                    if(not ma.textures.contains("map_Kd")) {
                        ma.textures["map_Kd"] = default_albedo;
                    }
                    if(not ma.textures.contains("map_Ke")) {
                        ma.textures["map_Ke"] = default_emissive;
                    }
                    if(not ma.textures.contains("map_Ks")) {
                        ma.textures["map_Ks"] = default_specular;
                    }
                }
            }
        }
    }
    if constexpr(false) {
        // auto& tmesh = *database.tmeshes.front();
        // for(uint32_t fi = 0; fi < face_count(tmesh); ++fi) {
        //     scene.all_faces.push_back(face(tmesh, fi));
        // }
        // for(auto& f : scene.all_faces) {
        //     for(uint32_t vi = 0; vi < incident_vertex_count(f); ++vi) {
        //         auto mid = geometry(tmesh).vertex_material_ids[index(incident_vertex(f, vi))];
        //         if(mid == 0) { // Light material.
        //             scene.emissive_faces.push_back(f);
        //         }
        //     }
        // }
    }

    { // Render passes.
        reload_shaders();
    }

    { // Camera.
        auto& c = *(camera = std::make_shared<eng::Camera>());
        if(auto pp = std::get_if<eng::PerspectiveProjection>(&c.projection)) {
            pp->aspect_ratio = 16.f / 9.f;
            pp->z_far = 100.f;
        }
        frustrum_camera = std::make_shared<eng::Camera>(*camera);
    }
    { // Ray tracer.
        ray_tracer.x_distribution = std::uniform_real_distribution<float>(-1.f, 1.f);
        ray_tracer.y_distribution = std::uniform_real_distribution<float>(-1.f, 1.f);
        ray_tracer.random = &random_generator;
    }
    { // Point pass.
        assign_program(point_pass,
            data::wavefront::point_program(shader_compiler));
    }
    { // Wireframe pass.
        assign_program(wireframe_pass, 
            data::shader::wireframe(shader_compiler));
    }
    { // Gizmos.
        bounding_box_gizmo = std::make_shared<eng::Mesh>(
            agl::engine::wireframe(gizmo::box_wireframe()));
    }
}
