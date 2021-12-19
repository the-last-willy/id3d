#pragma once

// Local headers.

#include "terrain/all.hpp"
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

struct App : Program {
    std::default_random_engine random_generator
    = std::default_random_engine(agl::standard::random_seed());

    eng::ShaderCompiler shader_compiler;
    
    eng::Camera camera;

    agl::engine::RenderPass mesh_pass;
    
    Terrain terrain;
    Forest forest;

    Settings settings;

    void init() override;

    void update(float) override;

    void render() override;
    void ui();
};
