#pragma once

#include "tree2/node/node.hpp"

#include <vector>

template<typename Child>
struct Swizzled : Node<Child> {
    std::vector<std::size_t> swizzling;

    constexpr
    Swizzled(Child c, std::vector<std::size_t> swizzling)
        : Node<Child>(std::move(c))
        , swizzling(std::move(swizzling))
    {}
};

template<typename Child>
Swizzled(Child, std::array<float, 3>) -> Swizzled<Child>;
