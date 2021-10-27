#pragma once

#include "intersection.hpp"
#include "ray_tracer.hpp"
#include "scene.hpp"

#include <optional>

inline
std::optional<Intersection> ray_trace(
    const Scene& s,
    const agl::engine::Ray& r)
{
    bool is_there = false;
    auto inter = Intersection();

    auto it = begin(s.all_faces);
    for(; it != end(s.all_faces); ++it) {
        auto tr = *it;
        auto h = intersection(r, tr);
        if(h and h->ray > 0.01f) {
            is_there = true;
            inter.hit = *h;
            inter.triangle = tr;
            break;
        }
    }
    for(; it != end(s.all_faces); ++it) {
        auto tr = *it;
        auto h = intersection(r, tr);
        if(h and h->ray > 0.01f and h->ray < inter.hit.ray) {
            inter.hit = *h;
            inter.triangle = tr;
        }
    }
    if(is_there) {
        return inter;
    } else {
        return std::nullopt;
    }
}
