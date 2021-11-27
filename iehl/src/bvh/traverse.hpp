#pragma once

#include "bvh.hpp"

template<typename F>
void traverse(const BvhNode& bn, F f, std::size_t depth) {
    { // Current.
        f(bn, depth);
    }
    // Inf.
    if(bn.inf_node) {
        traverse(*bn.inf_node, f, depth + 1);
    }
    // Sup.
    if(bn.sup_node) {
        traverse(*bn.sup_node, f, depth + 1);
    }
}

template<typename F>
void traverse(const Bvh& b, F f) {
    if(b.root_node) {
        traverse(*b.root_node, f, 0);
    }
}
