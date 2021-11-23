#pragma once

#include <agl/engine/all.hpp>

void Application::render() {
    clear(agl::default_framebuffer, agl::depth_tag, 1.f);

    auto nt = agl::engine::normal_transform(camera);
    auto wtc = agl::engine::world_to_clip(camera);

    uniform(scene.program, "world_to_clip", wtc);
    uniform(scene.program, "world_to_normal", nt);

    ::render(scene);
}
