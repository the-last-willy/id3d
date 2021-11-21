#pragma once

#include "predicate/orientation.hpp"
#include "insert.hpp"
#include "lawson.hpp"
#include "paraboloid.hpp"
#include "settings.hpp"
#include "walk_toward.hpp"

#include <agl/constant/all.hpp>

#include <vector>

inline
void ruppert_once(const RuppertSettings& rs, face_vertex::Mesh& m) {
    lawson(m);
    auto encroaching_edges = std::vector<face_vertex::TriangleVertexEdgeProxy>();
    for(auto&& t : triangles(m)) {
        auto&& to = topology(t);
        for(uint32_t i = 0; i < 3; ++i) {
            if(to->triangles[i] == index(t)) {
                encroaching_edges.push_back(proxy(m, t, to->vertices[i]));
            }
        }
    }
    for(uint32_t i = 0; i < size(topology(m).triangles); ++i) {
        auto&& t = triangle(m, i);
        if(smallest_angle(t) < rs.angle_threshold) {
            auto c = circumcenter(t).xy();
            auto wt = walk_toward(m, c);
            for(uint32_t j = 0; j < 3; ++j) {
                if(topology(t)->triangles[j] == index(t)) {
                    auto v = vertex(t, j);
                    auto p = position(v).xy();
                    auto v0 = vertex(t, j + 1);
                    auto v1 = vertex(t, j + 2);
                    auto p0 = position(v0).xy();
                    auto p1 = position(v1).xy();
                    auto center = (p0 + p1) / 2.f;
                    auto radius = length(p0 - center);
                    auto distance = length(p - center);
                    if(distance < radius) {
                        // std::cout << "v encroached" << std::endl;
                        auto s = split(proxy(m, t, v));
                        position(s) = paraboloid(center);
                        lawson(m);
                        return;
                    }
                }
            }
            for(auto& ee : encroaching_edges) {
                auto&& tr = incident_triangle(ee);
                auto&& v = opposite_vertex(ee);
                auto v0 = vertex(tr, relative_index(tr, v) + 1);
                auto v1 = vertex(tr, relative_index(tr, v) + 2);
                auto p0 = position(v0).xy();
                auto p1 = position(v1).xy();
                auto center = (p0 + p1) / 2.f;
                auto radius = length(p0 - center);
                auto distance = length(c - center);
                if(distance < radius) {
                    // std::cout << "c encroached" << std::endl;
                    auto s = split(ee);
                    position(s) = paraboloid(center);
                    lawson(m);
                    return;
                }
            }
            // std::cout << "not encroached" << std::endl;
            insert(m, c);
            lawson(m);
            return;
        }
    }
}
