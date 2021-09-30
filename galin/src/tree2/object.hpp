#pragma once

#include <sstream>

struct AnyObject {
    std::string name_;

    AnyObject(std::string&& name)
        : name_(std::move(name))
    {} 

    virtual ~AnyObject() {};

    std::string name() const {
        return name_;
    }
};

template<typename T>
struct Object : AnyObject {
    T distance;

    Object(std::string&& name, T&& distance)
        : AnyObject(std::move(name))
        , distance(std::move(distance))
    {}
};

template<typename T>
Object(std::string, T) -> Object<T>;

template<typename T>
std::string glsl_distance_fn(const Object<T>& o) {
    auto ss = std::stringstream();
    ss << "float " << o.name() << "_distance(vec3 position) {\n";
    ss << "\treturn " << glsl_expr(o.distance, "position") << ";\n";
    ss << "}\n";
    return ss.str();
}
