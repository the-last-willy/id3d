#pragma once

#include "scene/centroid.hpp"
#include "scene/scene.hpp"

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
    Bvh& b, BvhNode& n, auto first, auto last)
{
    // std::cout << "leaf " << std::distance(begin(b.leaves), first) << " " << std::distance(begin(b.leaves), last) << std::endl;
    n.index = b.node_count++;
    for(auto it = first; it != last; ++it) {
        it->node_index = n.index;
    }
    { // Compute bounds.
        auto& indices = scene(b).object_group.triangle_indices;
        auto& positions = scene(b).vertex_attribute_group.positions;
        n.bounds = agl::common::interval(positions[indices[first->primitive_index][0]]);
        for(auto it = first; it != last; ++it) 
        for(auto i : indices[it->primitive_index]) {
            extend(n.bounds, positions[i]);
        }
    }
}

inline
void construct_node(
    Bvh& b, BvhNode& n, auto first, auto last)
{

    n.index = b.node_count++;

    auto& s = scene(b);

    auto centroid_bounds = agl::common::interval(centroid(s, first->primitive_index));
    {
        for(auto it = first + 1; it != last; ++it) {
            extend(centroid_bounds, centroid(s, it->primitive_index));
        }
    }
    
    auto axis = std::size_t();
    auto midpoint = float();
    {
        auto l = length(centroid_bounds);
        axis = std::size_t(
            agl::standard::max_element_distance(begin(l), end(l)));
        midpoint = agl::common::midpoint(centroid_bounds)[axis];
    }

    auto centroid_less = [&](const BvhLeaf& l) {
        return centroid(s, l.primitive_index)[axis] < midpoint;
    };
    auto pivot = std::partition(
            first,
            last,
            centroid_less);

    { // Inf node.
        n.inf_node = std::make_unique<BvhNode>();
        if(std::distance(first, pivot) <= 1) {
            construct_leaf(b, *n.inf_node, first, pivot);
        } else {
            construct_node(b, *n.inf_node, first, pivot);
        }
    }
    { // Sup node.
        n.sup_node = std::make_unique<BvhNode>();
        if(std::distance(pivot, last) <= 1) {
            construct_leaf(b, *n.sup_node, pivot, last);
        } else {
            construct_node(b, *n.sup_node, pivot, last);
        }
    }
    { // Compute bounds.
        n.bounds = union_bounds(
            n.inf_node->bounds,
            n.sup_node->bounds);
    }
    // std::cout << "node " << std::distance(begin(b.leaves), first) << " " << std::distance(begin(b.leaves), last) << std::endl;
    // std::cout << "  inf " << lower_bound(n.bounds) << std::endl;
    // std::cout << "  sup " << upper_bound(n.bounds) << std::endl;
}

inline
Bvh bvh(const Scene& s) {
    auto b = Bvh();
    b.scene = &s;
    { // Leaves.
        b.leaves.resize(size(s.object_group.triangle_indices));
        for(std::size_t i = 0; i < size(b.leaves); ++i) {
            b.leaves[i].primitive_index = i;
        }
    }
    { // Nodes.
        b.root_node = std::make_unique<BvhNode>();
        construct_node(b, *b.root_node, begin(b.leaves), end(b.leaves));
    }
    return b;
}
