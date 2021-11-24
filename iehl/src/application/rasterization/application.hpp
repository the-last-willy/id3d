#pragma once

#include "scene/all.hpp"

#include <agl/standard/all.hpp>
#include <id3d/common/all.hpp>

#include <iostream>
#include <random>

//

struct Application : Program {
    eng::ShaderCompiler shader_compiler;

    eng::Camera camera;

    Scene scene;

    agl::engine::RenderPass wireframe_pass;
    std::shared_ptr<agl::engine::MeshInstance> bb_mesh;

    void init() override;

    void update(float) override;

    void render() override;
};
