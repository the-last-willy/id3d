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

    void init() override;

    void update(float) override;

    void render() override;
};
