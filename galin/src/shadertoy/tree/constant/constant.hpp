#pragma once

#include "tree2/node/node.hpp"

template<typename T>
struct Constant : Node<> {
    T value;

    Constant(T value)
        : value(value)
    {}
};

template<typename T>
Constant(T) -> Constant<T>;
