#pragma once

#include "node.hpp"

#include <sstream>

struct Object : Node {
    std::string id;

    Object(
        const std::string& id,
        const std::shared_ptr<Node>& child)
        : Node(child)
        , id(id)
    {}

    std::string name() const override {
        return id;
    }

    std::string glsl() const override {
        auto ss = std::stringstream();
        ss << "float " << name() << "(vec3 position) {\n";
        ss << "\treturn " << children[0].name() << "(" << position << ");\n";
        ss << "}\n";
        return ss.str();
    }
};
