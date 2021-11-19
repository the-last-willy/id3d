#pragma once

#include "insert.hpp"
#include "lawson.hpp"
#include "paraboloid.hpp"
#include "settings.hpp"
#include "walk_toward.hpp"

#include <agl/constant/all.hpp>

inline
void ruppert_once(const RuppertSettings& rs, face_vertex::Mesh& m) {
    for(uint32_t i = 0; i < size(topology(m).triangles); ++i) {
        auto&& t = triangle(m, i);
        if(smallest_angle(t) < rs.angle_threshold) {
            auto c = circumcenter(t).xy();
            auto wt = walk_toward(m, c);
            if(contains(wt, c)) {
                std::cout << "contained" << std::endl;
                insert(m, c);
            } else {
                std::cout << "encroached " << index(t) << std::endl;
                uint32_t j = 0;
                for(j = 0; j < 3; ++j) {
                    auto p0 = position(vertex(wt, j)).xy();
                    auto p1 = position(vertex(wt, (j + 1) % 3)).xy();
                    auto radius = length(p0 - p1) / 2.f;
                    auto center = (p0 + p1) / 2.f;
                    auto distance = length(c - center);
                    if(distance < radius) {
                        std::cout << j << std::endl;
                        auto edge = proxy(m, wt, vertex(wt, (j + 2) % 3));
                        auto s = split(edge);
                        position(s) = paraboloid(center);
                        break;
                    }
                }
                if(j == 3) {
                    j = 0 ;
                    std::cout << "what" << std::endl;
                    auto p0 = position(vertex(wt, j)).xy();
                    auto p1 = position(vertex(wt, (j + 1) % 3)).xy();
                    auto center = (p0 + p1) / 2.f;
                    auto edge = proxy(m, wt, vertex(wt, (j + 2) % 3));
                    auto s = split(edge);
                    position(s) = paraboloid(c);
                }
            }
            lawson(m);
            return;
        }
    }
}
