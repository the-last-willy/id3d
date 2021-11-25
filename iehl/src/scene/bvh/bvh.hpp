#pragma once

#include "centroid.hpp"
#include "scene.hpp"

#include <agl/common/all.hpp>
#include <agl/standard/all.hpp>

#include <memory>
#include <numeric>
#include <vector>

struct BvhLeaf {
    std::size_t node_index = 0;
    std::size_t primitive_index = 0;
};

struct BvhNode {
    std::size_t index;

    agl::common::Interval<agl::Vec3> bounds;

    std::unique_ptr<BvhNode> inf_node;
    std::unique_ptr<BvhNode> sup_node;
};

struct Bvh {
    const Scene* scene = nullptr;

    std::size_t node_count = 0;

    std::unique_ptr<BvhNode> root_node;
    std::vector<BvhLeaf> leaves;
};

inline
auto& scene(const Bvh& b) {
    return *b.scene;
}

inline
void construct_leaf(
    Bvh& b, BvhNode& n, std::size_t first, std::size_t last)
{
    std::cout << "leaf " << first << " " << last << std::endl;

    for(auto i = first; i < last; ++i) {
        b.leaves[i].node_index = n.index;
    }
}

inline
void construct_node(
    Bvh& b, BvhNode& n, std::size_t first, std::size_t last)
{
    std::cout << "node " << first << " " << last << std::endl;

    n.index = b.node_count++;

    auto& s = scene(b);

    auto centroid_bounds = agl::common::interval(centroid(s, first));
    std::cout << "c" << 0 << " " << centroid(s, first) << std::endl;
    std::cout << "inf " << lower_bound(centroid_bounds) << std::endl;
    std::cout << "sup " << upper_bound(centroid_bounds) << std::endl;
    {
        for(std::size_t i = first + 1; i < last; ++i) {
            std::cout << "c" << i << " " << centroid(s, i) << std::endl;
            extend(centroid_bounds, centroid(s, i));
        }
    }
    std::cout << "inf " << lower_bound(centroid_bounds) << std::endl;
    std::cout << "sup " << upper_bound(centroid_bounds) << std::endl;
    // // construire aussi l'englobant des triangles
    // BBox bounds= triangle_bounds(begin, end);
    
    auto axis = std::size_t();
    auto midpoint = float();
    {
        auto l = length(centroid_bounds);
        axis = std::size_t(
            agl::standard::max_element_distance(begin(l), end(l)));
        midpoint = agl::common::midpoint(centroid_bounds)[axis];
    }
    std::cout << "axis " << axis << std::endl;
    std::cout << "midpoint " << midpoint << std::endl;
    
    auto centroid_less = [&](const BvhLeaf& l) {
        return centroid(s, l.primitive_index)[axis] < midpoint;
    };
    auto pivot = first + std::size_t(std::distance(
        begin(b.leaves) + first, 
        std::partition(
            begin(b.leaves) + first,
            begin(b.leaves) + last,
            centroid_less)));
    std::cout << "pivot " << pivot << std::endl;

    n.inf_node = std::make_unique<BvhNode>();
    if(pivot <= first + 1) {
        construct_leaf(b, *n.inf_node, first, pivot);
    } else {
        construct_node(b, *n.inf_node, first, pivot);
    }

    n.sup_node = std::make_unique<BvhNode>();
    if(pivot >= last - 1) {
        construct_leaf(b, *n.sup_node, pivot, last);
    } else {
        construct_node(b, *n.sup_node, pivot, last);
    }
}

inline
Bvh bvh(const Scene& s) {
    auto b = Bvh();
    b.scene = &s;
    { // Leaves.
        b.leaves.resize(size(s.triangle_indices));
        for(std::size_t i = 0; i < size(b.leaves); ++i) {
            auto& l = b.leaves[i];
            l.primitive_index = i;
        }
    }
    { // Nodes.
        b.root_node = std::make_unique<BvhNode>();
        construct_node(b, *b.root_node, 0, size(b.leaves));
    }
    return b;
}
