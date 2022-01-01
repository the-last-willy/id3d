#pragma once

#include "bvh/all.hpp"
#include "forawrd_rendering/all.hpp"
#include "grid/all.hpp"
#include "lighting/all.hpp"
#include "settings/all.hpp"
#include "scene/all.hpp"
#include "statistics.hpp"

#include <agl/standard/all.hpp>
#include <id3d/common/all.hpp>

#include <iostream>
#include <random>

//

struct Application : Program {
    eng::ShaderCompiler shader_compiler;
    
    Scene scene;

    ForwardRenderer forward_renderer;
    gl::VertexArray forward_rendering_vao;



    eng::Camera camera;
    eng::Camera frustum_culling_camera;


    // agl::Buffer scene_triangle_material_id_buffer;

    Bvh scene_bvh;
    Grid scene_grid;

    agl::engine::RenderPass wireframe_pass;
    std::shared_ptr<agl::engine::MeshInstance> bb_mesh;

    std::shared_ptr<eng::Mesh> box_wireframe;

    Settings settings;
    Statistics statistics;

    void init() override;

    void update(float) override;

    void render() override;
};
