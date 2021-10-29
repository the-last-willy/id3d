#pragma once

#include "mesh/all.hpp"
#include "is_delaunay.hpp"

#include <agl/standard/vector/all.hpp>

#include <iostream>

inline
void lawson(face_vertex::Mesh& m) {
    auto v = std::vector<face_vertex::TriangleEdgeProxy>();
    // Remember all non-delaunay edges.
    for(auto&& t : triangles(m)) {
        for(uint32_t i = 0; i < adjacent_triangle_count(t); ++i) {
            auto at = adjacent_triangle(t, i);
            // Insure that edges are:
            // - interior 'index(at) != index(t)'
            // - not duplicated 'index(at) <= index(t)'
            if(index(at) < index(t)) {
                auto edge = proxy(mesh(t), index(t), index(at));
                if(not is_delaunay(edge)) {
                    v.push_back(std::move(edge));
                }
            }
        }
    }
    while(not empty(v)) {
        auto edge = agl::standard::take_back(v);
        if(is_valid(edge) and not is_delaunay(edge)) {
            flip(edge);
            auto t0 = adjacent_triangle(edge, 0);
            auto t1 = adjacent_triangle(edge, 1);
            auto t01 = adjacent_triangle(t0, relative_index(t0, t1) + 1);
            auto t02 = adjacent_triangle(t0, relative_index(t0, t1) + 2);
            auto t11 = adjacent_triangle(t1, relative_index(t1, t0) + 1);
            auto t12 = adjacent_triangle(t1, relative_index(t1, t0) + 2);
            auto e01 = proxy(m, index(t0), index(t01));
            auto e02 = proxy(m, index(t0), index(t02));
            auto e11 = proxy(m, index(t1), index(t11));
            auto e12 = proxy(m, index(t1), index(t12));
            if(index(t01) != index(t0) and not is_delaunay(e01)) {
                v.push_back(e01);
            }
            if(index(t02) != index(t0) and not is_delaunay(e02)) {
                v.push_back(e02);
            }
            if(index(t11) != index(t1) and not is_delaunay(e11)) {
                v.push_back(e11);
            }
            if(index(t12) != index(t1) and not is_delaunay(e12)) {
                v.push_back(e12);
            }
        }
    }
}

inline
void lawson_once(face_vertex::Mesh& m) {
    auto v = std::vector<face_vertex::TriangleEdgeProxy>();
    // Remember all non-delaunay edges.
    for(auto&& t : triangles(m)) {
        for(uint32_t i = 0; i < adjacent_triangle_count(t); ++i) {
            auto at = adjacent_triangle(t, i);
            // Insure that edges are:
            // - interior 'index(at) != index(t)'
            // - not duplicated 'index(at) <= index(t)'
            if(index(at) < index(t)) {
                auto edge = proxy(mesh(t), index(t), index(at));
                if(not is_delaunay(edge)) {
                    v.push_back(std::move(edge));
                }
            }
        }
    }
    while(not empty(v)) {
        auto edge = agl::standard::take_back(v);
        if(is_valid(edge) and not is_delaunay(edge)) {
            flip(edge);
        }
    }
}
