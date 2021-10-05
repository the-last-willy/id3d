#pragma once

#include "glsl.hpp"

#include <ostream>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

struct Node;
struct Operator;

using Children = std::vector<Node>;
using Operators = std::vector<std::shared_ptr<Operator>>;

struct Node {
    Operators operators;
    Children children;
};

std::string sdf_only(const Node& n, std::string argument);

std::string sdf_and_material(const Node& n, std::string argument);

void dump(const Node& n, std::ostream& os);

struct Operator {
    virtual ~Operator() = 0 {};

    virtual std::string sdf_only(
        const Children&,
        const std::string&) const
    {
        throw std::logic_error("Invalid operation.");
    }

    virtual std::string sdf_and_material(
        const Children&,
        const std::string&) const
    {
        throw std::logic_error("Invalid operation.");
    }

    virtual void dump(std::ostream&, const Node&) const {
        throw std::logic_error("Invalid operation.");
    }
};

template<typename Value>
struct Constant : Operator {
    Value value;

    std::string sdf_only(
        const Children&,
        const std::string&) const override
    {
        return glsl(value);
    }

    std::string sdf_and_material(
        const Children&,
        const std::string&) const override
    {
        return glsl(value);
    }
};

struct Material : Operator {
    std::array<float, 3> color;

    Material(std::array<float, 3> color)
        : color(color)
    {}

    std::string sdf_only(
        const Children& cn,
        const std::string& argument) const override
    {
        return ::sdf_only(cn.at(0), argument);
    }

    std::string sdf_and_material(
        const Children& cn,
        const std::string& argument) const override
    {
        auto ss = std::stringstream();
        ss << "DistanceAndMaterial("
        << sdf_only(cn, argument) << ", "
        << glsl(color)
        << ")";
        return ss.str();
    }
};

struct Naming : Operator {
    std::string name;

    Naming(std::string name)
        : name(std::move(name))
    {}

    std::string sdf_only(
        const Children&,
        const std::string& argument) const override
    {
        throw argument;
    }

    std::string sdf_and_material(
        const Children&,
        const std::string&) const override
    {
        throw std::logic_error("Not implemented.");
    }

    void dump_sdf_only(std::ostream& os, const Node& n) const {
        os << "float " << name << "_sdf(vec3 position) {\n"
        << "    return " << ::sdf_only(n, "position") << ";\n"
        << "}\n";
    }

    void dump_sdf_and_material(std::ostream& os, const Node& n) const {
        os << "DistanceAndMaterial " << name << "_material(vec3 position) {\n"
        << "    return " << ::sdf_and_material(n, "position") << ";\n"
        << "}\n";
    }

    void dump(std::ostream& os, const Node& n) const override {
        dump_sdf_only(os, n);
        dump_sdf_and_material(os, n);
    }
};

struct Separation : Operator {
    std::shared_ptr<Operator> sdf_operator;
    std::shared_ptr<Operator> material_operator;

    std::string sdf_only(
        const Children& cn,
        const std::string& argument) const override
    {
        return sdf_operator->sdf_only(cn, argument);
    }

    std::string sdf_and_material(
        const Children&, 
        const std::string&) const override
    {
        return "";
    }
};

struct Scaling : Operator {
    float scaling;
};

struct Sphere : Operator {
    float radius = 1.f;

    Sphere(float r)
        : radius(r)
    {}

    std::string sdf_only(
        const Children&,
        const std::string& argument) const override
    {
        auto ss = std::stringstream();
        ss << "(sdf_point("
        << argument << ") - "
        << radius << ")";
        return ss.str();
    }
};

struct Translation : Operator {

};

std::string sdf_only(const Node& n, std::string argument) {
    std::string s = argument;
    for(auto&& o : n.operators) {
        s = o->sdf_only(n.children, std::move(s));
    }
    return s;
}

std::string sdf_and_material(const Node& n, std::string argument) {
    std::string s = argument;
    for(auto&& o : n.operators) {
        s = o->sdf_and_material(n.children, std::move(s));
    }
    return s;
}

void dump(const Node& n, std::ostream& os) {
    n.operators.at(0)->dump(os, n);
}
