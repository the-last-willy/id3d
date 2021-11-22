// Disabled warnings.

#pragma warning(disable : 4005 4996)

// Local headers.

#include "data/all.hpp"
#include "ray/all.hpp"
#include "settings.hpp"
#include "settings/all.hpp"
#include "statistics/all.hpp"

#include <agl/format/gltf2/all.hpp>
#include <agl/format/wavefront/all.hpp>
#include <agl/standard/all.hpp>
#include <id3d/common/all.hpp>

// External libraries.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <range/v3/view/indirect.hpp>
#include <range/v3/view/map.hpp>

// Standard library.

#include <iostream>
#include <random>

//

inline
auto create_random_generator() {
    auto rd = std::random_device();
    return std::default_random_engine(rd());
}

struct Object {
    std::shared_ptr<agl::engine::TriangleMesh> cpu_mesh;
    std::shared_ptr<eng::Mesh> gpu_mesh;

    // Model space.
    agl::engine::BoundingBox bounding_box;
};

struct RasterizationScene {
    std::vector<Object> objects;
    std::vector<std::shared_ptr<eng::Material>> materials;
};

struct GltfProgram : Program {
    eng::ShaderCompiler shader_compiler = {};

    RasterizationScene rasterization_scene;

    // Active camera.
    std::shared_ptr<eng::Camera> camera = {};
    std::shared_ptr<eng::Camera> frustrum_camera = {};

    bool ambient_pass_loaded = false;
    agl::engine::RenderPass ambient_pass;
    bool blinn_phong_pass_loaded = false;
    agl::engine::RenderPass blinn_phong_pass;

    agl::engine::RenderPass point_pass;
    
    float time = 0.f;

    RayTracer ray_tracer;
    Scene scene;

    int ray_n = 0;

    agl::engine::TriangleMesh inter_mesh;

    bool toggle_rasterization = true;
    
    std::default_random_engine random_generator = create_random_generator();

    RayTracingSettings ray_tracing_settings;

    agl::engine::RenderPass wireframe_pass;

    std::shared_ptr<eng::Mesh> bounding_box_gizmo;

    AllSettings settings;
    AllStatistics statistics;

    void reload_shaders() {
        try {
            ambient_pass.program = std::make_shared<eng::Program>(
                data::wavefront::forward_ambient_program(shader_compiler));
            ambient_pass_loaded = true;
        } catch(...) {
            ambient_pass.program.reset();
            std::cerr << "AMBIENT SHADERS FAILED TO COMPILE." << std::endl << std::endl;
            ambient_pass_loaded = false;
        }
        try {
            blinn_phong_pass.program = std::make_shared<eng::Program>(
                data::wavefront::forward_blinn_phong_program(shader_compiler));
            blinn_phong_pass_loaded = true;
        } catch(...) {
            blinn_phong_pass.program.reset();
            std::cerr << "BLINN PHONG SHADERS FAILED TO COMPILE." << std::endl << std::endl;
            blinn_phong_pass_loaded = false;
        }
    }

    bool new_mesh = true;

    void reload_points() {
        if(vertex_count(inter_mesh) > 100'000) {
            // If enough points. Leave the previous mesh and work on a new one.
            inter_mesh = agl::engine::TriangleMesh();
            new_mesh = true;
        } else {
            if(not new_mesh) {
                point_pass.subscriptions.pop_back();
            }
            new_mesh = false;
            auto render_inter_mesh = std::make_shared<eng::Mesh>(
                agl::engine::point_mesh(inter_mesh, rasterization_scene.materials));
            subscribe(point_pass, render_inter_mesh);
        }
    }

    void init() override;

    void update(float) override;

    void rtx() {
        auto ro = camera->view.position;
        
        auto distrib01 = std::uniform_real_distribution<float>(0.f, 1.f);
        auto norm_distrib = std::uniform_real_distribution<float>(-1.f, 1.f);

        float n = float(ray_tracing_settings.ray_per_frame);
        for(float i = 0; i < n; ++i) {    

            auto x = norm_distrib(random_generator) * ray_tracing_settings.range;
            auto y = norm_distrib(random_generator) * ray_tracing_settings.range;

            auto rd = (agl::engine::rotation(*camera) * agl::vec4(x, y, -1.f, 0.f)).xyz(); 

            auto r = agl::engine::Ray(ro, rd);
            
            auto inter = ray_trace(scene, r);
            if(inter) {
                auto v = create_vertex(inter_mesh);
                position(v) = inter->hit.position;
                auto ivert = incident_vertex(inter->triangle, 0);
                normal(v) = normal(ivert);
                auto lightened = false;
                auto light_f = scene.emissive_faces.front();
                auto light_p = agl::vec3(0.f);
                {
                    auto r0 = distrib01(random_generator);
                    auto r1 = distrib01(random_generator);
                    if(r0 + r1 > 1.f) {
                        r0 = 1.f - r0;
                        r1 = 1.f - r1;
                    }
                    light_p = r0 * position(incident_vertex(light_f, 0))
                    + r1 * position(incident_vertex(light_f, 1))
                    + (1.f - r0 - r1) * position(incident_vertex(light_f, 2));
                }
                auto light_r = agl::engine::Ray(position(v), light_p - position(v));
                auto lh = ray_trace(scene, light_r);
                if(lh) {
                    if(length(lh->hit.position - light_p) < 0.01) {
                        lightened = true;
                    }
                }
                auto lambertian = 0.f;
                if(lightened) {
                    auto light_dif = light_p - position(v);
                    auto d_to_l_2 = dot(light_dif);
                    lambertian = std::max(dot(normal(v), normalize(light_dif)), 0.f) * 30.f / d_to_l_2;
                }
                bool is_emissive = false;
                for(auto& ef : scene.emissive_faces) {
                    if(index(ef) == index(inter->triangle)) {
                        is_emissive = true;
                    }
                }
                if(is_emissive) {
                    lambertian = 1.f;
                }
                auto mid = material_id(ivert);
                auto& m = rasterization_scene.materials[mid];
                if(auto ptr = dynamic_cast<eng::Uniform<agl::Vec3>*>(m->uniforms["Kd"])) {
                    color(v) = lambertian * ptr->value;
                }
            }
        }
    }

    void update_render_passes() {
        clear(ambient_pass);
        clear(blinn_phong_pass);
        clear(wireframe_pass);

        auto wtc = agl::engine::world_to_clip(*camera);
        { // Update wireframe pass.
            // for(auto& o : rasterization_scene.objects) {
            //     auto& m = *o.cpu_mesh;
            //     auto bb = bounding_box(m);
            //     auto&& instance = subscribe(wireframe_pass, bounding_box_gizmo);
            //     assign_uniform(
            //         *instance,
            //         "world_to_clip",
            //         wtc * gizmo::box_wireframe_model_to_world(bb));
            // }
            { // Frustrum gizmo.
                auto&& instance = subscribe(wireframe_pass, bounding_box_gizmo);
                instance->uniforms["rgb"]
                    = std::make_shared<eng::Uniform<agl::Vec3>>(agl::vec3(1.f, 1.f, 1.f));
                assign_uniform(
                    *instance,
                    "world_to_clip",
                    wtc * gizmo::box_wireframe_model_to_world(
                        agl::engine::bounding_box(*frustrum_camera)));
            }
        }
        if(settings.frustrum_culling.enabled) { // Frustrum culling.
            statistics.frustrum_culling.accepted_count = 0;
            statistics.frustrum_culling.rejected_count = 0;
            auto fbb = agl::engine::bounding_box(*frustrum_camera);
            for(auto& o : rasterization_scene.objects) {
                auto& obb = o.bounding_box;
                if(are_intersecting(obb, fbb)) {
                    statistics.frustrum_culling.accepted_count += 1;
                    if(ambient_pass_loaded) {
                        subscribe(ambient_pass, o.gpu_mesh);
                    }
                    if(blinn_phong_pass_loaded) {
                        subscribe(blinn_phong_pass, o.gpu_mesh);
                    }
                    if(settings.frustrum_culling.draw_rejected_bounding_boxes) {
                        auto&& instance = subscribe(wireframe_pass, bounding_box_gizmo);
                        assign_uniform(
                            *instance,
                            "world_to_clip",
                            wtc * gizmo::box_wireframe_model_to_world(obb));
                        instance->uniforms["rgb"]
                        = std::make_shared<eng::Uniform<agl::Vec3>>(agl::vec3(0.f, 1.f, 0.f));
                    }
                } else {
                    statistics.frustrum_culling.rejected_count += 1;
                    if(settings.frustrum_culling.draw_rejected_bounding_boxes) {
                        auto&& instance = subscribe(wireframe_pass, bounding_box_gizmo);
                        assign_uniform(
                            *instance,
                            "world_to_clip",
                            wtc * gizmo::box_wireframe_model_to_world(obb));
                        instance->uniforms["rgb"]
                        = std::make_shared<eng::Uniform<agl::Vec3>>(agl::vec3(1.f, 0.f, 0.f));
                    }
                    if(settings.frustrum_culling.draw_rejected_objects) {
                        if(ambient_pass_loaded) {
                            subscribe(ambient_pass, o.gpu_mesh);
                        }
                        if(blinn_phong_pass_loaded) {
                            subscribe(blinn_phong_pass, o.gpu_mesh);
                        }
                    }
                }
            }
        } else {
            statistics.frustrum_culling.accepted_count = 0;
            statistics.frustrum_culling.rejected_count = 0;
            for(auto& o : rasterization_scene.objects) {
                if(ambient_pass_loaded) {
                    subscribe(ambient_pass, o.gpu_mesh);
                }
                if(blinn_phong_pass_loaded) {
                    subscribe(blinn_phong_pass, o.gpu_mesh);
                }
            }
        }
    }

    void render() override;
};

#include "init.hpp"
#include "render.hpp"
#include "update.hpp"

void throwing_main() {
    auto p = GltfProgram();
    run(p);
}

int main() {
    try {
        throwing_main();
        return 0;
    } catch(const std::exception& e) {
        std::cerr << "std::exception: " << e.what() << std::endl;
    } catch(...) {
        std::cerr << "Unhandled exception." << std::endl;
    }
    return -1;
}
