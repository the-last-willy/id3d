#pragma once

#include "bvh/all.hpp"
#include "forward_rendering/all.hpp"
#include "frustum_culler/all.hpp"
#include "gizmo/all.hpp"
#include "hdr_framebuffer/all.hpp"
#include "occlusion_culling/all.hpp"
#include "scene/all.hpp"
#include "solid_renderer/all.hpp"
#include "tone_mapping/all.hpp"
#include "wireframe_renderer/all.hpp"
#include "z_prepasser/all.hpp"
#include "settings.hpp"
#include "statistics.hpp"

#include <agl/standard/all.hpp>
#include <id3d/common/all.hpp>

#include <iostream>
#include <random>

//

struct Application : Program {
    eng::ShaderCompiler shader_compiler;
    
    // Scene.

    Scene scene;

    gl::VertexArray scene_z_prepasser_vao;

    //

    HDR_Framebuffer hdr_framebuffer;

    //

    ForwardRenderer forward_renderer;
    gl::VertexArray forward_rendering_vao;

    Z_Prepasser z_prepasser;

    // Compute shaders.

    FrustumCuller frustum_culler;
    OcclusionCuller occlusion_culler;

    //

    ToneMapper tone_mapper;
    gl::VertexArray tone_mapping_vao;

    eng::Camera camera;
    eng::Camera frustum_culling_camera;
    eng::Camera occlusion_culling_camera;

    LightCulling light_culling;

    // Gizmos.

    SolidRenderer solid_renderer;
    WireframeRenderer wireframe_renderer;

    gl::VertexArray scene_solid_vao;

    SolidBoxObject solid_box;
    gl::VertexArray solid_box_solid_renderer_vao;

    WireBoxObject wire_box;
    gl::VertexArray wire_box_vao;

    WireQuadObject wire_quad;
    gl::VertexArray wire_quad_vao;

    //

    Settings settings;
    Statistics statistics;

    void init() override;

    void update(float) override;

    void render() override;
};
