#pragma once

#include "tree/node.hpp"

#include <array>
#include <memory>
#include <sstream>

struct Scaled : Node {
    float scaling;

    Scaled(
        const std::shared_ptr<Node>& child,
        float scaling
    )
        : Node(child)
        , scaling(scaling)
    {}

    std::string name() const override {
        return children[0]->name() + "_scaled";
    }

    std::string glsl() const override {
        auto ss = std::stringstream();
        ss << "float " << name() << "(vec3 position) {\n";
        ss << "\treturn " << children[0]->name() << "(position / " << scaling << ") * " << scaling << ";\n";
        ss << "}\n";
        return ss.str();
    }
};
