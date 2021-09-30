#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

struct Node {
    std::vector<std::shared_ptr<Node>> children;

    virtual ~Node() {};

    template<typename... Ts>
    Node(Ts&&... args)
        : children{std::forward<Ts>(args)...}
    {}

    virtual std::string name() const {
        throw std::logic_error("Not implemented.");
    };

    // GLSL.

    virtual std::string glsl() const {
        throw std::logic_error("Not implemented.");
    }
};

inline
auto begin(const Node& n) {
    return begin(n.children);
}

inline
auto end(const Node& n) {
    return end(n.children);
}
