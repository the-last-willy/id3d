#pragma once

#include "mesh/all.hpp"

inline
void laplacian(face_vertex::Mesh& mesh) {
    { // Compute Laplacian.
        for (auto v : vertices(mesh)) {
            auto vertex_area = 0.f;
            auto sum = agl::vec3(0.f);
            for(auto t : adjacent_triangles(v)) {
                for(uint32_t i = 0; i < 3; ++i) {
                    if(index(vertex(t, i)) == index(v)) {
                        auto ui = position(vertex(t, i));
                        auto uj = position(vertex(t, (i + 1) % vertex_count(t)));
                        auto uk = position(vertex(t, (i + 2) % vertex_count(t)));
                        auto triangle_area = length(cross(ui - uk, uj - uk));
                        auto aij = dot(ui - uk, uj - uk) / triangle_area;
                        auto aik = dot(ui - uj, uk - uj) / triangle_area;
                        sum += aij * (uj - ui);
                        sum += aik * (uk - ui);
                    }
                }
                vertex_area += area(t);
            }
            vertex_area /= 3.f;
            auto laplacian = sum / (2.f * vertex_area);
            float mean_curvature = length(laplacian) / 2.f;
            color(v) = agl::vec3(1.f - 1.f / (1.f + mean_curvature / 20.f));
            normal(v) = -normalize(laplacian);
        }
    }
    { // Flip incorrect normals.
        for(auto&& t : triangles(mesh)) {
            auto v0 = vertex(t, 0);
            auto v1 = vertex(t, 1);
            auto v2 = vertex(t, 2);
            auto tn = normalize(cross(
                position(v1) - position(v0),
                position(v2) - position(v0)));
            if(dot(normal(v0), tn) < 0.f) {
                normal(v0) = -normal(v0);
            }
            if(dot(normal(v1), tn) < 0.f) {
                normal(v1) = -normal(v1);
            }
            if(dot(normal(v2), tn) < 0.f) {
                normal(v2) = -normal(v2);
            }
        }
    }

}