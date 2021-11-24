#pragma once

#include <agl/engine/all.hpp>

void Application::render() {
    clear(agl::default_framebuffer, agl::depth_tag, 1.f);

    auto nt = agl::engine::normal_transform(camera);
    auto wtc = agl::engine::world_to_clip(camera);

    {   clear(wireframe_pass);
        bb_mesh = agl::standard::shared(
            agl::engine::instance(
                agl::engine::wireframe(
                    gizmo::box_wireframe())));
        auto bb = bounding_box(scene);
        subscribe(wireframe_pass, bb_mesh);
        bb_mesh->uniforms["color"]
        = std::make_shared<eng::Uniform<agl::Vec4>>(
            agl::vec4(0.f, 1.f, 0.f, 1.f));
        bb_mesh->uniforms["model_to_clip"]
        = std::make_shared<eng::Uniform<agl::Mat4>>(
            wtc * gizmo::box_wireframe_model_to_world(bb));
        agl::engine::render(wireframe_pass);
    }

    {
        uniform(scene.program, "world_to_clip", wtc);
        uniform(scene.program, "world_to_normal", nt);

        ::render(scene);
    }
}
