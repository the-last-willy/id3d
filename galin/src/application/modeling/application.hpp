#pragma once

// Local headers.

#include "modelling/all.hpp"
#include "settings.hpp"

#include <agl/engine/all.hpp>
#include <agl/standard/all.hpp>
#include <id3d/common/all.hpp>

// External libraries.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard library.

#include <fstream>
#include <iostream>
#include <random>

//

struct CubicBezierMesh {
    std::vector<std::array<uint32_t, 16>> patches;
    std::vector<agl::Vec3> vertices;
};

CubicBezierMesh load_teapot() {
    auto file = std::ifstream("galin/data/teaset/teapot");
    if(not file.is_open()) {
        throw std::runtime_error("Failed to open teapot.");
    }
    auto cbm = CubicBezierMesh();
    { // Patches.
        uint32_t patch_count;
        file >> patch_count;
        cbm.patches.resize(patch_count);
        for(auto& p : cbm.patches) {
            file >> p[0];
            p[0] -= 1;
            for(uint32_t i = 1; i < 16; ++i) {
                file.ignore(1); // ','
                file >> p[i];
                p[i] -= 1;
            }
        }
    }
    { // Vertices.
        uint32_t vertex_count;
        file >> vertex_count;
        cbm.vertices.resize(vertex_count);
        for(auto& v : cbm.vertices) {
            file >> v[0];
            for(uint32_t i = 1; i < 3; ++i) {
                file.ignore(1); // ','
                file >> v[i];
            }
            std::swap(v[1], v[2]);
        }
    }
    return cbm;
}

struct Object2 {
    std::shared_ptr<agl::engine::TriangleMesh> cpu_tessellated_mesh;

    std::shared_ptr<agl::engine::MeshInstance> gpu_control_mesh;
    std::shared_ptr<agl::engine::MeshInstance> gpu_tessellated_mesh;
};

struct Scene {
    agl::engine::BoundingBox cpu_bounding_box;
    std::shared_ptr<agl::engine::MeshInstance> gpu_bounding_box;
};

struct App : Program {
    std::default_random_engine random_generator
    = std::default_random_engine(agl::standard::random_seed());

    Settings settings;

    eng::ShaderCompiler shader_compiler;
    
    eng::Camera camera;

    agl::engine::RenderPass mesh_pass;
    agl::engine::RenderPass wireframe_pass;

    std::shared_ptr<eng::Mesh> sphere_gizmo;

    std::shared_ptr<agl::engine::MeshInstance> transform_aoe;

    Object object;

    std::optional<FFD> ffd;
    std::shared_ptr<agl::engine::MeshInstance> ffd_control_mesh;

    void init() override;

    void update(float) override;
    void update_object();

    void render() override;
    void ui();

    void generate_ffd() {

        ffd = ::ffd(
            {
                std::size_t(settings.ffd.resolution[0]),
                std::size_t(settings.ffd.resolution[1]),
                std::size_t(settings.ffd.resolution[2])
            },
            object.bounding_box);
        { // Control mesh.
            ffd_control_mesh = agl::standard::shared(
                agl::engine::instance(
                    agl::engine::wireframe(
                        control_mesh3(ffd->control_points))));
        }
    }

};
