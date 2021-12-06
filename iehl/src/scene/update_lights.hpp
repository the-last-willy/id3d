#pragma once

#include "scene.hpp"

#include <span>

inline
void update_lights(
    Scene& s,
    const agl::Mat4& world_to_view)
{
    auto transformed_lights = s.lights;
    for(auto& l : transformed_lights) {
        l.position = world_to_view * l.position;
        l.position /= l.position[3];
    }
    if(s.light_ssbo) {
        delete_(s.light_ssbo);
    }
    s.light_ssbo = agl::create(agl::buffer_tag);
    storage(s.light_ssbo, std::span(transformed_lights));
}
