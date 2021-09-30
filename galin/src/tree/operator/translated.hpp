#pragma once

#include "tree/node.hpp"

#include <array>
#include <memory>
#include <sstream>

struct Translated : Node {
    std::array<float, 3> translation;

    Translated(
        const std::shared_ptr<Node>& child,
        std::array<float, 3> translation
    )
        : Node(child)
        , translation(translation)
    {}

    std::string name() const override {
        return children[0]->name() + "_translated";
    }

    std::string glsl() const override {
        auto ss = std::stringstream();
        ss << "float " << name() << "(vec3 position) {\n";
        ss << "\treturn " << children[0]->name() << "(position - "
        << "vec3(" << translation[0] << ", " << translation[1] << ", " << translation[2] << "));\n";
        ss << "}\n";
        return ss.str();
    }
};
