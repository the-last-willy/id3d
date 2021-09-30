#pragma once

#include "tree/node.hpp"

struct Point : Node {
    ~Point() = default;

    std::string name() const override {
        return std::string("point");
    }

    std::string glsl() const override {
        return std::string(
            "float sdf_point(vec3 v) {\n"
            "\treturn length(v);\n"
            "}\n");
    }
};
