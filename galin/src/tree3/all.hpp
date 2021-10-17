#pragma once

#include "glsl.hpp"

#include <array>
#include <memory>
#include <ostream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <string>

struct DumpInfo {
    std::ostream* os;


};

struct Node {
    virtual std::size_t operator_count() const = 0;
    virtual std::size_t primitive_count() const = 0;

    virtual std::string sdf_only(const std::string&) const {
        throw std::logic_error("Not implemented.");
    }

    virtual std::string sdf_and_material(const std::string&) const {
        throw std::logic_error("Not implemented.");
    }

    virtual void dump_sdf_only(std::ostream&) const {}

    virtual void dump_sdf_and_material(std::ostream&) const {}
};

using Children = std::vector<std::shared_ptr<Node>>;
using SharedNode = std::shared_ptr<Node>;

struct Branch : Node {
    std::vector<SharedNode> children;

    Branch() = default;

    Branch(SharedNode child)
        : children()
    {
        children.push_back(std::move(child));
    }

    Branch(std::vector<SharedNode>&& children)
        : children(std::move(children))
    {}

    std::size_t operator_count() const override {
        if(empty(children)) {
            return 0;
        } else {
            auto sum = std::size_t(1);
            for(auto&& c : children) {
                sum += c->operator_count();
            }
            return sum;
        }
    }

    std::size_t primitive_count() const override {
        if(empty(children)) {
            return 1;
        } else {
            auto sum = std::size_t(0);
            for(auto&& c : children) {
                sum += c->primitive_count();
            }
            return sum;
        }
    }

    std::string sdf_only(const std::string& s) const override {
        if(size(children) == 1) {
            return children.at(0)->sdf_only(s);
        } else {
            throw std::logic_error("Not implemented.");
        }
    }

    std::string sdf_and_material(const std::string& s) const override {
        if(size(children) == 1) {
            return children.at(0)->sdf_and_material(s);
        } else {
            throw std::logic_error("Not implemented.");
        }
    }

    void dump_sdf_only(std::ostream& os) const override {
        for(auto&& c : children) {
            c->dump_sdf_only(os);
        }
    };

    void dump_sdf_and_material(std::ostream& os) const override {
        for(auto&& c : children) {
            c->dump_sdf_and_material(os);
        }
    }
};

struct Attraction : Branch {
    float intensity = 1.f;

    Attraction(float intensity, SharedNode child)
        : Branch({std::move(child)})
        , intensity(intensity)
    {}

    std::string sdf_only(const std::string& s) const override {
        return children.at(0)->sdf_only(
            "attracted(\n" + s + ",\n" + glsl(intensity) + ")");
    }

    std::string sdf_and_material(const std::string& s) const override {
        return children.at(0)->sdf_and_material(
            "attracted(\n" + s + ",\n" + glsl(intensity) + ")");
    }
};

struct Controlled : Branch {
    Controlled(SharedNode child)
        : Branch(std::move(child))
    {}

    std::string sdf_only(const std::string& s) const override {
        return children.at(0)->sdf_only("controlled(" + s + ")");
    }

    std::string sdf_and_material(const std::string& s) const override {
        return children.at(0)->sdf_and_material("controlled(" + s + ")");
    }
};

inline
SharedNode controlled(SharedNode child) {
    return std::make_shared<Controlled>(std::move(child));
}

struct Difference : Branch {
    Difference(SharedNode sn0, SharedNode sn1)
        : Branch({std::move(sn0), std::move(sn1)})
    {}

    std::string sdf_only(const std::string& s) const override {
        return "difference("
        + children.at(0)->sdf_only(s) + ", "
        + children.at(1)->sdf_only(s) + ")";
    }

    std::string sdf_and_material(const std::string& s) const override {
        return "difference("
        + children.at(0)->sdf_and_material(s) + ", "
        + children.at(1)->sdf_and_material(s) + ")";
    }
};

inline
SharedNode difference(SharedNode sn0, SharedNode sn1) {
    return std::make_shared<Difference>(std::move(sn0), std::move(sn1));
}

struct Dilatation : Branch {
    float radius = 0.f;

    Dilatation(float radius, SharedNode child)
        : Branch({std::move(child)})
        , radius(radius)
    {}

    std::string sdf_only(const std::string& s) const override {
        return "dilated(" + glsl(radius) + ",\n" + children.at(0)->sdf_only(s) + ")";
    }

    std::string sdf_and_material(const std::string& s) const override {
        return "dilated(" + glsl(radius) + ",\n" + children.at(0)->sdf_and_material(s) + ")";
    }
};

inline
SharedNode dilated(float radius, SharedNode sn) {
    return std::make_shared<Dilatation>(radius, std::move(sn));
}

struct Intersection : Branch {
    Intersection(SharedNode sn0, SharedNode sn1)
        : Branch({std::move(sn0), std::move(sn1)})
    {}

    Intersection(Children cn)
        : Branch(std::move(cn))
    {}

    std::string sdf_only(const std::string& s) const override {
        auto r = children.at(0)->sdf_only(s);
        for(std::size_t i = 1; i < size(children); ++i) {
            r = "intersection(\n"
            + children.at(i)->sdf_only(s) + ",\n"
            + r + ")";
        }
        return r;
    }

    std::string sdf_and_material(const std::string& s) const override {
        auto r = children.at(0)->sdf_and_material(s);
        for(std::size_t i = 1; i < size(children); ++i) {
            r = "intersection(\n"
            + children.at(i)->sdf_and_material(s) + ",\n"
            + r + ")";
        }
        return r;
    }
};

struct Inverted : Branch {
    Inverted(SharedNode child)
        : Branch({std::move(child)})
    {}

    std::string sdf_only(const std::string& s) const override {
        return "inverted(\n" + children.at(0)->sdf_only(s) + ")";
    }

    std::string sdf_and_material(const std::string& s) const override {
        return "inverted(\n" + children.at(0)->sdf_and_material(s) + ")";
    }
};

inline
SharedNode inverted(SharedNode sn) {
    return std::make_shared<Inverted>(std::move(sn));
}

template<typename... SharedNodes>
SharedNode intersection(SharedNodes... sns) {
    return std::make_shared<Intersection>(
        std::vector<SharedNode>{std::move(sns)...});
}

struct Material : Branch {
    std::array<float, 3> color;

    Material(std::array<float, 3> color, SharedNode child)
        : Branch({std::move(child)})
        , color(color)
    {}

    std::string sdf_and_material(const std::string& s) const override {
        return "SdfAndMaterial("
        + glsl(color) + ",\n"
        + sdf_only(s) + ")";
    }
};

inline
SharedNode material(std::array<float, 3> color, SharedNode sn) {
    return std::make_shared<Material>(color, std::move(sn));
}

inline
SharedNode material(float r, float g, float b, SharedNode sn) {
    return std::make_shared<Material>(std::array{r, g, b}, std::move(sn));
}

struct Object : Branch {
    std::string name;

    Object(std::string name, SharedNode child)
        : Branch({std::move(child)})
        , name(std::move(name))
    {}

    std::string sdf_only(const std::string& s) const override {
        return name + "_sdf(\n" + s + ")";
    }

    std::string sdf_and_material(const std::string& s) const override {
        return name + "_material(\n" + s + ")";
    }

    void dump_sdf_only(std::ostream& os) const override {
        Branch::dump_sdf_only(os);
        os << "float " << name << "_sdf(vec3 position) {\n"
        << "    return " << children.at(0)->sdf_only("position") << ";\n"
        << "}\n\n";
    }

    void dump_sdf_and_material(std::ostream& os) const override {
        Branch::dump_sdf_and_material(os);
        os << "SdfAndMaterial " << name << "_material(vec3 position) {\n"
        << "    return " << children.at(0)->sdf_and_material("position") << ";\n"
        << "}\n\n";
    }
};

inline
SharedNode named(std::string name, SharedNode sn) {
    return std::make_shared<Object>(std::move(name), std::move(sn));
}

struct Onion : Branch {
    Onion(SharedNode child)
        : Branch({std::move(child)})
    {}

    std::string sdf_only(const std::string& s) const override {
        return "onion(\n" + children.at(0)->sdf_only(s) + ")";
    }

    std::string sdf_and_material(const std::string& s) const override {
        return "onion(\n" + children.at(0)->sdf_and_material(s) + ")";
    }
};

SharedNode onion(SharedNode sn) {
    return std::make_shared<Onion>(std::move(sn));
}

struct Reflected : Branch {
    int axis = 0;

    Reflected(int axis, SharedNode child)
        : Branch({std::move(child)})
        , axis(axis)
    {}

    std::string sdf_only(const std::string& s) const override {
        auto reflections = std::array{"reflected_x", "reflected_y", "reflected_z"};
        return children.at(0)->sdf_only(
            reflections[axis] + ("(\n" + s + ")"));
    }

    std::string sdf_and_material(const std::string& s) const override {
        auto reflections = std::array{"reflected_x", "reflected_y", "reflected_z"};
        return children.at(0)->sdf_and_material(
            reflections[axis] + ("(\n" + s + ")"));
    }
};

inline
SharedNode reflected(int axis, SharedNode sn) {
    return std::make_shared<Reflected>(axis, std::move(sn));
}

struct RotatedX : Branch {
    float angle = 0.f;

    RotatedX(float angle, SharedNode child)
        : Branch({std::move(child)})
        , angle(angle)
    {}

    std::string sdf_only(const std::string& s) const override {
        if(angle == 0.f) {
            return children.at(0)->sdf_only(s);
        } else {
            return children.at(0)->sdf_only(
                "rotated_x(" + glsl(angle) + ",\n" + s + ")");
        }
    }

    std::string sdf_and_material(const std::string& s) const override {
        if(angle == 0.f) {
            return children.at(0)->sdf_and_material(s);
        } else {
            return children.at(0)->sdf_and_material(
                "rotated_x(" + glsl(angle) + ",\n" + s + ")");
        }
    }
};

inline
SharedNode rotated_x(float angle, SharedNode sn) {
    return std::make_shared<RotatedX>(angle, std::move(sn));
}

struct RotatedY : Branch {
    float angle = 0.f;

    RotatedY(float angle, SharedNode child)
        : Branch({std::move(child)})
        , angle(angle)
    {}

    std::string sdf_only(const std::string& s) const override {
        if(angle == 0.f) {
            return children.at(0)->sdf_only(s);
        } else {
            return children.at(0)->sdf_only(
                "rotated_y(" + glsl(angle) + ",\n" + s + ")");
        }
    }

    std::string sdf_and_material(const std::string& s) const override {
        if(angle == 0.f) {
            return children.at(0)->sdf_and_material(s);
        } else {
            return children.at(0)->sdf_and_material(
                "rotated_y(" + glsl(angle) + ",\n" + s + ")");
        }
    }
};

inline
SharedNode rotated_y(float angle, SharedNode sn) {
    return std::make_shared<RotatedY>(angle, std::move(sn));
}

struct RotatedZ : Branch {
    float angle = 0.f;

    RotatedZ(float angle, SharedNode child)
        : Branch({std::move(child)})
        , angle(angle)
    {}

    std::string sdf_only(const std::string& s) const override {
        if(angle == 0.f) {
            return children.at(0)->sdf_only(s);
        } else {
            return children.at(0)->sdf_only(
                "rotated_z(" + glsl(angle) + ",\n" + s + ")");
        }
    }

    std::string sdf_and_material(const std::string& s) const override {
        if(angle == 0.f) {
            return children.at(0)->sdf_and_material(s);
        } else {
            return children.at(0)->sdf_and_material(
                "rotated_z(" + glsl(angle) + ",\n" + s + ")");
        }
    }
};

inline
SharedNode rotated_z(float angle, SharedNode sn) {
    return std::make_shared<RotatedZ>(angle, std::move(sn));
}

struct Scaling : Branch {
    std::array<float, 3> scaling = {1.f, 1.f, 1.f};

    Scaling(std::array<float, 3> scaling, SharedNode child)
        : Branch({std::move(child)})
        , scaling(scaling)
    {}

    std::string sdf_only(const std::string& s) const override {
        auto ms = scaling[0];
        for(auto sc : scaling) {
            ms = std::min(ms, std::abs(sc));
        }
        return "corrected("
        + glsl(ms) + ",\n"
        + children.at(0)->sdf_only(
            "scaled(" + glsl(scaling) + ",\n" + s + ")")
        + ")";
    }

    std::string sdf_and_material(const std::string& s) const override {
        auto ms = scaling[0];
        for(auto sc : scaling) {
            ms = std::min(ms, std::abs(sc));
        }
        return "corrected("
        + glsl(ms) + ",\n"
        + children.at(0)->sdf_and_material(
            "scaled(" + glsl(scaling) + ",\n" + s + ")")
        + ")";
    }
};

inline
SharedNode scaled(float x, float y, float z, SharedNode sn) {
    return std::make_shared<Scaling>(std::array{x, y, z}, std::move(sn));
}

inline
SharedNode scaled(float s, SharedNode sn) {
    return scaled(s, s, s, std::move(sn));
}

struct SmoothUnion : Branch {
    float radius;

    SmoothUnion(float r, SharedNode sn0, SharedNode sn1)
        : Branch({std::move(sn0), std::move(sn1)})
        , radius(r)
    {}

    SmoothUnion(float r, Children cn)
        : Branch(std::move(cn))
        , radius(r)
    {}

    std::string sdf_only(const std::string& s) const override {
        auto r = children.at(0)->sdf_only(s);
        for(std::size_t i = 1; i < size(children); ++i) {
            r = "smooth_union("
            + glsl(radius) + ",\n"
            + children.at(i)->sdf_only(s) + ",\n"
            + r + ")";
        }
        return r;
    }

    std::string sdf_and_material(const std::string& s) const override {
        auto r = children.at(0)->sdf_and_material(s);
        for(std::size_t i = 1; i < size(children); ++i) {
            r = "smooth_union("
            + glsl(radius) + ",\n"
            + children.at(i)->sdf_and_material(s) + ",\n"
            + r + ")";
        }
        return r;
    }
};

template<typename... SharedNodes>
SharedNode smooth_union(float r, SharedNodes... sns) {
    return std::make_shared<SmoothUnion>(
        r, std::vector<SharedNode>{std::move(sns)...});
}

struct Swizzling : Branch {
    std::array<int, 3> swizzling = {0, 1, 2};

    Swizzling(std::array<int, 3> swizzling, SharedNode child)
        : Branch({std::move(child)})
        , swizzling(swizzling)
    {}

    std::string sdf_only(const std::string& s) const override {
        auto symbols = std::array{'x', 'y', 'z'};
        auto ss = s + ".";
        for(auto sw : swizzling) {
            ss += symbols[sw];
        }
        return children.at(0)->sdf_only(ss);
    }

    std::string sdf_and_material(const std::string& s) const override {
        auto symbols = std::array{'x', 'y', 'z'};
        auto ss = s + ".";
        for(auto sw : swizzling) {
            ss += symbols[sw];
        }
        return children.at(0)->sdf_and_material(ss);
    }
};

struct Translation : Branch {
    std::array<float, 3> translation;

    Translation(std::array<float, 3> translation, SharedNode child)
        : Branch({std::move(child)})
        , translation(translation)
    {}

    std::string sdf_only(const std::string& s) const override {
        return children.at(0)->sdf_only(
            "translated(" + glsl(translation) + ",\n" + s + ")");
    }

    std::string sdf_and_material(const std::string& s) const override {
        return children.at(0)->sdf_and_material(
            "translated(" + glsl(translation) + ",\n" + s + ")");
    }
};

inline
SharedNode translated(float x, float y, float z, SharedNode sn) {
    return std::make_shared<Translation>(std::array{x, y, z}, std::move(sn));
}

// struct UniformScaling : Branch {
//     float scaling = 1.f;

//     UniformScaling(float scaling, SharedNode child)
//         : Branch({std::move(child)})
//         , scaling(scaling)
//     {}

//     std::string sdf_only(const std::string& s) const override {
//         return "scaling_out(\n"
//         + children.at(0)->sdf_only(
//             "scaling_in(\n" + s + ",\n" + glsl(scaling) + ")")
//         + ", " + glsl(scaling) + ")";
//     }

//     std::string sdf_and_material(const std::string& s) const override {
//         return "scaling_out(\n"
//         + children.at(0)->sdf_and_material(
//             "scaling_in(\n" + s + ",\n" + glsl(scaling) + ")")
//         + ", " + glsl(scaling) + ")";
//     }
// };

struct Union : Branch {
    Union(SharedNode sn0, SharedNode sn1)
        : Branch({std::move(sn0), std::move(sn1)})
    {}

    Union(Children cn)
        : Branch(std::move(cn))
    {}

    std::string sdf_only(const std::string& s) const override {
        auto r = children.at(0)->sdf_only(s);
        for(std::size_t i = 1; i < size(children); ++i) {
            r = "unionn(\n"
            + children.at(i)->sdf_only(s) + ",\n"
            + r + ")";
        }
        return r;
    }

    std::string sdf_and_material(const std::string& s) const override {
        auto r = children.at(0)->sdf_and_material(s);
        for(std::size_t i = 1; i < size(children); ++i) {
            r = "unionn(\n"
            + children.at(i)->sdf_and_material(s) + ",\n"
            + r + ")";
        }
        return r;
    }
};

template<typename... SharedNodes>
SharedNode unionn(SharedNodes... sns) {
    return std::make_shared<Union>(
        std::vector<SharedNode>{std::move(sns)...});
}

struct Leaf : Node {
    std::size_t operator_count() const override {
        return 0;
    }

    std::size_t primitive_count() const override {
        return 1;
    }
};

struct Circle : Leaf {
    float radius = 1.f;

    Circle(float r)
        : radius(r)
    {}

    std::string sdf_only(const std::string& s) const override {
        return "circle(" + glsl(radius) + ",\n" + s + ")";
    }
};

inline
SharedNode circle(float r) {
    return std::make_shared<Circle>(r);
}

struct Cone : Leaf {
    float height = 1.f;
    float radius = 1.f;

    Cone(float h, float r)
        : height(h)
        , radius(r)
    {}

    std::string sdf_only(const std::string& s) const override {
        return "cone(vec2(" + glsl(radius) + "," + glsl(height) + "),\n" + s + ")";
    }
};

inline
SharedNode cone(float height, float radius) {
    return std::make_shared<Cone>(height, radius);
}

struct Cube : Leaf {
    std::string sdf_only(const std::string& s) const override {
        return "cube(\n" + s + ")";
    }
};

inline
SharedNode cube() {
    return std::make_shared<Cube>();
}

struct Ellipsoid : Leaf {
    std::array<float, 3> radiuses = {1.f, 1.f, 1.f};

    Ellipsoid(std::array<float, 3> radiuses)
        : radiuses(radiuses)
    {}

    std::string sdf_only(const std::string& s) const override {
        return "ellipsoid("
        + glsl(radiuses) + ",\n"
        + s + ")";
    }
};

inline
SharedNode ellipsoid(float r0, float r1, float r2) {
    return std::make_shared<Ellipsoid>(std::array{r0, r1, r2});
}

struct Half : Leaf {
    int axis = 0;

    Half(int axis)
        : axis(axis)
    {}

    std::string sdf_only(const std::string& s) const override {
        constexpr auto symbols = std::array{'x', 'y', 'z'};
        return s + "." + symbols[axis];
    }
};

inline
SharedNode half(int axis) {
    return std::make_shared<Half>(axis);
}

struct LineSegment : Leaf {
    std::array<float, 3> position0;
    std::array<float, 3> position1;

    LineSegment(
        std::array<float, 3> position0,
        std::array<float, 3> position1)
        : position0(position0)
        , position1(position1)
    {}

    std::string sdf_only(const std::string& s) const override {
        return "line_segment("
        + glsl(position0) + ", "
        + glsl(position1) + ", "
        + s + ")";
    }
};

inline
SharedNode line_segment(std::array<float, 3> position0, std::array<float, 3> position1) {
    return std::make_shared<LineSegment>(position0, position1);
}

struct Plane : Leaf {
    float width = 1.f;

    Plane(float width)
        : width(width)
    {}

    std::string sdf_only(const std::string& s) const override {
        return "(plane(" + s + ") - " + glsl(width / 2.f) + ")";
    }
};

struct Sphere : Leaf {
    float radius = 1.f;

    Sphere(float r)
        : radius(r)
    {}

    std::string sdf_only(const std::string& s) const override {
        return "dilated(" + glsl(radius) + ",\n"
            + "point(" + s + ")"
        + ")";
    }
};

inline
SharedNode sphere(float r) {
    return std::make_shared<Sphere>(r);
}
