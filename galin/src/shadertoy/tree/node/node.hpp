#pragma once

#include <tuple>

template<typename... Children>
struct Node {
    std::tuple<Children...> children;

    template<typename... Ts>
    Node(Ts&&... ts)
        : children(std::forward<Ts>(ts)...)
    {}
};

template<typename... Children>
Node(Children...) -> Node<Children...>;
